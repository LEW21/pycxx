#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace pycxx
{
	namespace token_tree
	{
		using std::string;
		using std::vector;
		using std::ostream;
		using std::endl;

		struct Token
		{
			enum Type {
				Other,
				Atom,
				Identifier,
				Round,
				Square,
				Curly
			} type;
			string code;
			vector<Token> subtokens;

			operator string() {return code;}
		};

		inline bool operator==(const Token& a, const Token& b) {return a.type == b.type && a.code == b.code && a.subtokens == b.subtokens;}
		inline bool operator!=(const Token& a, const Token& b) {return !(a == b);}

		using Tokens = vector<Token>;

		struct Statement
		{
			vector<Token> tokens;
			vector<Statement> block;
		};

		inline bool operator==(const Statement& a, const Statement& b) {return a.tokens == b.tokens && a.block == b.block;}
		inline bool operator!=(const Statement& a, const Statement& b) {return !(a == b);}

		using Statements = vector<Statement>;

		inline void formatted_print(ostream& out, const Token& t, int indent = 0)
		{
			for (auto&& i = 0; i < indent; ++i)
				out << "\t";
			out << "Token" << t.type << " "<< t.code << "" << endl;

			for (auto&& st : t.subtokens)
				formatted_print(out, st, indent+1);
		}

		inline void formatted_print(ostream& out, const Statement& s, int indent = 0)
		{
			for (auto&& i = 0; i < indent; ++i)
				out << "\t";
			out << "Statement:" << endl;

			for (auto&& t : s.tokens)
				formatted_print(out, t, indent+1);

			for (auto&& sb : s.block)
				formatted_print(out, sb, indent+1);
		}
	}
}
