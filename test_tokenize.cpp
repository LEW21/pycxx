#include "pycxx/tokenize.hpp"

#include <cassert>

using std::cout;
using pycxx::tokenize;
using pycxx::token_tree::Statements;
using pycxx::token_tree::Statement;
using pycxx::token_tree::Token;

void test_tokenizer()
{
	auto b = tokenize("def a(x, y):\n\tx=5\nx=\"abcde\"");

	for (auto s : b)
		formatted_print(cout, s);

	auto correct_b = Statements{
		Statement{
			{
				{Token::Identifier, "def"}, {Token::Identifier, "a"}, {Token::Round, "(x, y)",
					{
						{Token::Identifier, "x"}, {Token::Other, ","}, {Token::Identifier, "y"}
					}
				}, {Token::Other, ":"}
			},
			{
				Statement{
					{
						{Token::Identifier, "x"}, {Token::Other, "="}, {Token::Atom, "5"}
					}
				}
			}
		},
		Statement{
			{
				{Token::Identifier, "x"}, {Token::Other, "="}, {Token::Atom, "\"abcde\""}
			}
		},
	};

	assert(b == correct_b);
}

int main()
{
	test_tokenizer();
}
