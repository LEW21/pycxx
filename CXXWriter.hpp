#include <str.hpp>

using std::string;

struct CXXWriter
{
	string code = "#include <pycxx_runtime.hpp>\n\n";

	string ns;
	int indent = 0;

	void enterNS(string n)
	{
		code += "namespace " + str::replace(n, '.', " { namespace ") + " {\n\n";
		ns = n;
	}

	void write(string c)
	{
		for (auto&& i = 0; i < indent; ++i)
			code += "\t";
		code += c + "\n";
	}

	void begin()
	{
		write("{");
		++indent;
	}

	void end()
	{
		--indent;
		write("}");
		code += "\n";
	}

	void close()
	{
		for (auto&& i = 0; i < str::count(ns, '.') + 1; ++i)
			code += "}";
		code += "\n";
	}
};
