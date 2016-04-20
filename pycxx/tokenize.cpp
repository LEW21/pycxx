#include "tokenize.hpp"

#include <str.hpp>
#include <boost/optional.hpp>
#include <regex>

using std::string;
using std::vector;
using std::move;
using boost::optional;
/*constexpr*/ const auto nullopt = boost::none;

using pycxx::token_tree::Token;
using pycxx::token_tree::Statement;

#include "errors.hpp"
using pycxx::errors::SyntaxError;
using pycxx::errors::IndentationError;
using pycxx::errors::TabError;

#include <pycxx_runtime.hpp>
using pycxx_runtime::freeze;

auto newlines = std::string{"\n\r"};
auto spaces = std::string{" \f\n\r\t\v"};
auto special = std::string{"()[]{}'\"?:;,.-@=<>"};

bool is_newline[256];
bool is_space[256];
bool is_special[256];

void init_is_space_special()
{
	for (auto i = 0; i < 256; ++i)
	{
		is_newline[i] = false;
		is_space[i] = false;
		is_special[i] = false;
	}

	for (auto c : newlines)
		is_newline[c] = true;

	for (auto c : spaces)
		is_space[c] = true;

	for (auto c : special)
		is_special[c] = true;
}

template <typename F>
void findT(const std::string& buffer, int& offset, F predicate)
{
	bool backslashed = false;
	bool found = false;

	for (; offset < buffer.size(); ++offset)
	{
		if (backslashed)
		{
			backslashed = false;
			continue;
		}

		char c = buffer[offset];
		if (predicate(c))
		{
			found = true;
			break;
		}
		else if (c == '\\')
		{
			backslashed = true;
		}
	}

	if (!found)
		offset = -1;
}

void find(const char& wanted, const std::string& buffer, int& offset)
{
	findT(buffer, offset, [=](char c){return c == wanted;});

	if (offset != -1)
		++offset;
}

void findBoundary(const std::string& buffer, int& offset)
{
	findT(buffer, offset, [=](char c){return is_space[c] || is_special[c];});
}

void findEndl(const std::string& buffer, int& offset)
{
	findT(buffer, offset, [=](char c){return c == '\n' || c == '\r';});

	if (offset != -1)
		++offset;
}

class Tokenizer
{
	string data;
	int pos = 0;

	string prefix;
	void read_prefix();

	template <char terminator>
	inline auto tokenize_single() -> optional<Token>;

	template <char terminator>
	inline auto tokenize_until() -> vector<Token>;

public:
	Tokenizer(string data): data(move(data)) {}
	auto tokenize_statement() -> vector<Token>;
	auto tokenize_block(string block_prefix) -> vector<Statement>;
};

namespace pycxx
{
	auto tokenize(const string& data) -> vector<Statement>
	{
		auto&& t = Tokenizer{data};
		return t.tokenize_block("");
	}
}

auto&& cxx_string = std::regex{"^(?:u8|u|U|L)?\"(?:[^\n\"\\\\]|\\\\['\"?\\\\a-zA-Z0-9])*\""};
auto&& longstring = std::regex{"^(?:u8|u|U|L)?\"\"\"([^\"\\\\]|\\\\['\"?\\\\a-zA-Z0-9])*\"\"\""};
auto&& integer    = std::regex{"^[0-9]+"};

template <char terminator>
auto Tokenizer::tokenize_single() -> optional<Token>
{
	// let begin = int(pos)
	auto&& begin = freeze(int(pos));

	// TODO handle ud-suffix
	auto match = std::smatch{};
	if (std::regex_search(data.cbegin() + pos, data.cend(), match, cxx_string))
	{
		pos += match[0].length();
		return Token{Token::Atom, match[0].str()};
	}
	if (std::regex_search(data.cbegin() + pos, data.cend(), match, longstring))
	{
		pos += match[0].length();
		return Token{Token::Atom, match[0].str()};
	}
	if (std::regex_search(data.cbegin() + pos, data.cend(), match, integer))
	{
		pos += match[0].length();
		return Token{Token::Atom, match[0].str()};
	}

	// let first = data[pos++]
	auto&& first = freeze(data[pos++]);

	assert(!is_space[first]);

	if (first == terminator)
		return nullopt;

	if (terminator == '\n' && is_newline[first])
		return nullopt;

	switch (first)
	{
		case '(':
		{
			auto&& subtokens = tokenize_until<')'>();
			return Token{Token::Round, str::slice(data, begin, pos), subtokens};
		}

		case '[':
		{
			auto&& subtokens = tokenize_until<']'>();
			return Token{Token::Square, str::slice(data, begin, pos), subtokens};
		}

		case '{':
		{
			auto&& subtokens = tokenize_until<'}'>();
			return Token{Token::Curly, str::slice(data, begin, pos), subtokens};
		}

		case ')':
		case ']':
		case '}':
			throw SyntaxError("invalid syntax");

		case '-':
			if (pos < data.size())
				switch (data[pos])
				{
					case '>':
						++pos;
				}
			break;
	}

	if (!is_special[first])
	{
		findBoundary(data, pos);

		if (pos == -1)
			pos = data.size();
	}

	if (begin == pos)
	{
		if (terminator == '\n')
			return nullopt;
		else
			throw SyntaxError("unexpected EOF while parsing");
	}

	if (is_special[first])
		return Token{Token::Other, str::slice(data, begin, pos)};
	else
		return Token{Token::Identifier, str::slice(data, begin, pos)};
}

template <char terminator>
auto Tokenizer::tokenize_until() -> vector<Token>
{
	auto&& tokens = vector<Token>{};

	while (pos < data.size())
	{
		for (; pos < data.size() && is_space[data[pos]]; ++pos)
		{
			if (terminator == '\n')
			{
				if (tokens.size() && is_newline[data[pos]])
				{
					++pos;
					return tokens;
				}
			}
		}

		if (pos < data.size())
		{
			auto&& t = tokenize_single<terminator>();
			if (!t)
				break;
			tokens.emplace_back(*t);
		}
	}

	return tokens;
}

auto Tokenizer::tokenize_statement() -> vector<Token>
{
	return tokenize_until<'\n'>();
}

void Tokenizer::read_prefix()
{
	auto&& begin = int(pos);

	for (; pos < data.size() && is_space[data[pos]]; ++pos)
	{
		if (is_newline[data[pos]])
			begin = pos + 1;
	}

	prefix = str::slice(data, begin, pos);
}

auto Tokenizer::tokenize_block(string block_prefix) -> vector<Statement>
{
	init_is_space_special();

	auto&& block = vector<Statement>{};

	while (pos < data.size())
	{
		auto stmt = Statement{tokenize_statement()};

		read_prefix();

		if (stmt.tokens[stmt.tokens.size()-1].code == ":")
		{
			if (!str::startswith(prefix, block_prefix))
				throw TabError("inconsistent use of tabs and spaces in indentation");
			else if (prefix.size() <= block_prefix.size())
				throw IndentationError("expected an indented block");

			stmt.block = tokenize_block(prefix);
		}

		block.emplace_back(move(stmt));

		if (prefix != block_prefix)
		{
			if (prefix.size() < block_prefix.size())
				break;
			else if (!str::startswith(prefix, block_prefix))
				throw TabError("inconsistent use of tabs and spaces in indentation");
			else if (prefix.size() != block_prefix.size())
				throw IndentationError("unexpected indent");
		}
	}

	return block;
}
