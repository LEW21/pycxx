#pragma once

#include "token_tree.hpp"
#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <variantxx/variant.hpp>
#include "copy_ptr.hpp"

namespace pycxx
{
	namespace ast
	{
		using std::move;
		using std::string;
		using std::vector;
		using boost::optional;
		/*constexpr*/ const auto nullopt = boost::none;
		using std::nullptr_t;

		// The most important Python type
		struct ellipsis {};

		inline bool operator==(const ellipsis&, const ellipsis&) { return true; }
		inline bool operator!=(const ellipsis& a, const ellipsis& b) { return !(a == b); }

		// Trivial exprs ------------------------------------------------------
		using Const = xx::variant<nullptr_t, ellipsis, bool>;

		struct Atom // int / double / char / string - and they can have C++'s suffixes
		{
			string code;
		};

		inline auto operator ""_atom(unsigned long long v)           {return Atom{std::to_string(v)};}
		inline auto operator ""_atom(long double v)                  {return Atom{std::to_string(v)};}
		inline auto operator ""_atom(char v)                         {return Atom{string{"'"} + v + "'"};}
		inline auto operator ""_atom(const char* v, std::size_t len) {return Atom{string{"\""} + string{v, len} + "\""};}

		inline bool operator==(const Atom& a, const Atom& b) { return a.code == b.code; }
		inline bool operator!=(const Atom& a, const Atom& b) { return !(a == b); }

		struct Identifier
		{
			string name;
		};

		inline Identifier operator ""_id(const char* str, std::size_t len)
		{
			return Identifier{string{str, len}};
		}

		inline bool operator==(const Identifier& a, const Identifier& b) { return a.name == b.name; }
		inline bool operator!=(const Identifier& a, const Identifier& b) { return !(a == b); }

		// Expressions --------------------------------------------------------
		struct Call;
		struct Subscript;
		struct Property;

		using Expr = xx::variant<nullptr_t, ellipsis, bool, Atom, Identifier, Call, Subscript, Property>;
		using Exprs = vector<Expr>;

		struct Call
		{
			indirect<Expr> self;
			indirect<Exprs> arguments;
		};

		struct Subscript
		{
			indirect<Expr> self;
			indirect<Exprs> arguments;
		};

		struct Property
		{
			indirect<Expr> self;
			string name;
		};

		inline auto operator ""_expr(unsigned long long v)           {return Expr{operator ""_atom(v)};}
		inline auto operator ""_expr(long double v)                  {return Expr{operator ""_atom(v)};}
		inline auto operator ""_expr(char v)                         {return Expr{operator ""_atom(v)};}
		inline auto operator ""_expr(const char* v, std::size_t len) {return Expr{operator ""_atom(v, len)};}

		static const auto True = Expr{true};
		static const auto False = Expr{false};
		static const auto None = Expr{nullptr};
		static const auto DotDotDot = Expr{ellipsis{}};

		inline bool operator==(const Call& a, const Call& b) { return a.self == b.self && a.arguments == b.arguments; }
		inline bool operator!=(const Call& a, const Call& b) { return !(a == b); }

		inline bool operator==(const Subscript& a, const Subscript& b) { return a.self == b.self && a.arguments == b.arguments; }
		inline bool operator!=(const Subscript& a, const Subscript& b) { return !(a == b); }

		inline bool operator==(const Property& a, const Property& b) { return a.self == b.self && a.name == b.name; }
		inline bool operator!=(const Property& a, const Property& b) { return !(a == b); }

		// Patterns -----------------------------------------------------------
		struct PatIdent;
		struct PatTuple;

		using Pat = xx::variant<PatIdent, PatTuple>;
		using Pats = vector<Pat>;

		struct PatIdent
		{
			bool is_ref = false;
			bool is_mutable = false;
			string name;
		};

		struct PatTuple
		{
			indirect<Pats> idents;
		};

		inline bool operator==(const PatIdent& a, const PatIdent& b) { return a.name == b.name && a.is_mutable == b.is_mutable && a.is_ref == b.is_ref; }
		inline bool operator!=(const PatIdent& a, const PatIdent& b) { return !(a == b); }

		inline bool operator==(const PatTuple& a, const PatTuple& b) { return a.idents == b.idents; }
		inline bool operator!=(const PatTuple& a, const PatTuple& b) { return !(a == b); }

		// Statements ---------------------------------------------------------
		struct Pass;
		struct FuncDecl;
		struct LetDecl;
		struct ExprStatement;

		using Statement = xx::variant<Pass, LetDecl, FuncDecl, ExprStatement>;
		using Block = vector<Statement>;

		struct Pass
		{
		};

		struct TypedPat
		{
			Pat pat;
			optional<Expr> type;
		};

		struct LetDecl
		{
			TypedPat pat;
			optional<Expr> value;
		};

		struct ParamDecl
		{
			TypedPat pat;
			optional<Expr> default_value;
		};

		using ParamDecls = vector<ParamDecl>;

		struct FuncDecl
		{
			bool is_mutable = false;
			string name;
			ParamDecls params;
			optional<Expr> return_type;
			indirect<Block> code;
		};

		struct Loop
		{
			indirect<Block> code;
		};

		struct ForLoop
		{
			TypedPat pat;
			Expr value;
			indirect<Block> code;
		};

		struct ExprStatement
		{
			Expr expr;
		};

		inline bool operator==(const Pass&, const Pass&) { return true; }
		inline bool operator!=(const Pass& a, const Pass& b) { return !(a == b); }

		inline bool operator==(const TypedPat& a, const TypedPat& b) { return a.pat == b.pat && a.type == b.type; }
		inline bool operator!=(const TypedPat& a, const TypedPat& b) { return !(a == b); }

		inline bool operator==(const ParamDecl& a, const ParamDecl& b) { return a.pat == b.pat && a.default_value == b.default_value; }
		inline bool operator!=(const ParamDecl& a, const ParamDecl& b) { return !(a == b); }

		inline bool operator==(const LetDecl& a, const LetDecl& b) { return a.pat == b.pat && a.value == b.value; }
		inline bool operator!=(const LetDecl& a, const LetDecl& b) { return !(a == b); }

		inline bool operator==(const FuncDecl& a, const FuncDecl& b) { return a.is_mutable == b.is_mutable && a.name == b.name && a.params == b.params && a.return_type == b.return_type && a.code == b.code; }
		inline bool operator!=(const FuncDecl& a, const FuncDecl& b) { return !(a == b); }

		inline bool operator==(const Loop& a, const Loop& b) { return a.code == b.code; }
		inline bool operator!=(const Loop& a, const Loop& b) { return !(a == b); }

		inline bool operator==(const ForLoop& a, const ForLoop& b) { return  a.pat == b.pat && a.value == b.value && a.code == b.code; }
		inline bool operator!=(const ForLoop& a, const ForLoop& b) { return !(a == b); }

		inline bool operator==(const ExprStatement& a, const ExprStatement& b) { return a.expr == b.expr; }
		inline bool operator!=(const ExprStatement& a, const ExprStatement& b) { return !(a == b); }
	}
}
