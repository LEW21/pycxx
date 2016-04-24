#include "pycxx/tokenize.hpp"
#include "pycxx/parse.hpp"
#include <boost/range/adaptor/transformed.hpp>

#include "CXXWriter.hpp"
#include <fstream>
#include <unordered_map>

using std::string;
using std::vector;
using boost::adaptors::transformed;
using namespace pycxx;

struct Builtin
{
	string name;
};

const auto builtins = std::unordered_map<string, Builtin>{
	{"print", {"pycxx_runtime::print"}},
	{"int", {"int"}},
};

auto translate_expr(ast::Expr expr) -> string
{
	if (expr.is<std::nullptr_t>())
		return "nullptr";
	else if (expr.is<ast::ellipsis>())
		return "pycxx_runtime::DotDotDot";
	else if (expr.is<bool>())
		return expr.get<bool>() ? "true" : "false";
	else if (expr.is<ast::Atom>())
		return expr.get<ast::Atom>().code;
	else if (expr.is<ast::Identifier>())
	{
		auto&& id = expr.get<ast::Identifier>().name;
		// TODO map names

		try
		{
			return builtins.at(id).name;
		}
		catch (std::out_of_range&) {}

		// TODO throw error
		return id;
	}

	else if (expr.is<ast::Call>())
	{
		auto&& call = expr.get<ast::Call>();
		return translate_expr(*call.self) + "(" + str::join(", ", *call.arguments | transformed(translate_expr)) + ")";
	}
	else if (expr.is<ast::Subscript>())
	{
		auto&& subscript = expr.get<ast::Subscript>();
		return translate_expr(*subscript.self) + "[" + str::join(", ", *subscript.arguments | transformed(translate_expr)) + "]";
	}
	else if (expr.is<ast::Property>())
	{
		auto&& property = expr.get<ast::Property>();
		if (property.name[0] >= '0' && property.name[0] <= '9')
			return "std::get<" + property.name + ">(" + translate_expr(*property.self) + ")";

		return translate_expr(*property.self) + "." + property.name;
	}

	return "???"; // TODO throw
}

void translate_block(CXXWriter& out, ast::Block block)
{
	for (auto&& s : block)
	{
		if (s.is<ast::FuncDecl>())
		{
			// TODO support param deconstruction

			auto&& f = s.get<ast::FuncDecl>();

			auto&& auto_params = vector<string>{};
			for (auto&& param : f.params)
			{
				if (!param.pat.type)
					auto_params.emplace_back(param.pat.pat.get<ast::PatIdent>().name);
			}

			if (auto_params.size())
				out.write("template <typename T" + str::join(", typename T", auto_params) + ">");

			auto&& cxx = string{};
			cxx += "inline auto " + f.name + "(";
			bool first = true;
			for (auto&& param : f.params)
			{
				if (first)
					first = false;
				else
					cxx += ", ";

				if (param.pat.type)
					cxx += translate_expr(*param.pat.type);
				else
					cxx += "T" + param.pat.pat.get<ast::PatIdent>().name + "&&";
				cxx += " " + param.pat.pat.get<ast::PatIdent>().name;

				if (param.default_value)
					cxx += " = " + translate_expr(*param.default_value);
			}
			cxx += ")";

			if (f.return_type)
				cxx += " -> " + translate_expr(*f.return_type);

			if (!auto_params.size() && f.return_type)
			{
				// This function is fully specified!
				// We can hide it in the .cpp file.
			}

			out.write(cxx);

			out.begin();
			if (f.code)
				translate_block(out, *f.code);
			out.end();
		}
		else if (s.is<ast::LetDecl>())
		{
			auto&& l = s.get<ast::LetDecl>();

			assert(l.value); // TODO handle value-less lets

			if (l.pat.pat.is<ast::PatTuple>())
			{
				// Convert
				// let (a, b, c) = (1, 2, 3)
				// to
				// auto&& temp = (1, 2, 3)
				// auto&& a = std::get<0>(temp);
				// auto&& b = std::get<1>(temp);
				// auto&& c = std::get<2>(temp);
			}

			auto&& pat = l.pat.pat.get<ast::PatIdent>();

			auto&& cxx = string{};
			cxx += "auto&& " + pat.name + " = ";
			if (!pat.is_mutable)
				cxx += "pycxx_runtime::freeze(";
			if (l.pat.type)
				cxx += translate_expr(*l.pat.type) + "{";
			cxx += translate_expr(*l.value);
			if (l.pat.type)
				cxx += "}";
			if (!pat.is_mutable)
				cxx += ")";

			out.write(cxx + ";");
		}
		else if (s.is<ast::Loop>())
		{
			auto&& l = s.get<ast::Loop>();

			out.write("for (;;)");
			out.begin();
			if (l.code)
				translate_block(out, *l.code);
			out.end();
		}
		else if (s.is<ast::ForLoop>())
		{
			auto&& l = s.get<ast::ForLoop>();

			if (l.pat.pat.is<ast::PatTuple>())
			{
				// TODO
			}

			auto&& pat = l.pat.pat.get<ast::PatIdent>();

			out.write("for (auto&& " + pat.name + " : " + translate_expr(l.expr) + ")");

			out.begin();
			if (l.code)
				translate_block(out, *l.code);
			out.end();
		}
		else if (s.is<ast::ExprStatement>())
		{
			auto&& expr = s.get<ast::ExprStatement>().expr;

			out.write(translate_expr(expr) + ";");
		}
	}
}

auto translate_module(string ns, string src) -> string
{
	auto&& out = CXXWriter{};

	out.enterNS(ns);

	translate_block(out, parse(tokenize(src)));

	out.close();

	return out.code;
};

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << translate_module("pycxx.example", "def x(typed: int, generic):\n\tlet x: int = 15") << std::flush;
		return 1;
	}

	auto&& in = std::ifstream{str::replace(argv[1], '.', "/") + ".pycxx"};
	auto&& src = std::string{std::istreambuf_iterator<char>{in}, std::istreambuf_iterator<char>{}};

	std::cout << translate_module(argv[1], src);
}
