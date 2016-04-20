#pragma once

#include "token_stream.hpp"

namespace pycxx
{
	namespace Brackets
	{
		struct Round;
		struct Square;
		struct Curly;
	}

	template <> struct ParsingResultT<Brackets::Round>  { using type = token_tree::Tokens; };
	template <> struct ParsingResultT<Brackets::Square> { using type = token_tree::Tokens; };
	template <> struct ParsingResultT<Brackets::Curly>  { using type = token_tree::Tokens; };

	template<>
	inline auto TokenStream::is<Brackets::Round>() -> bool
	{
		token.expect("(");
		return !is_end() && token->code[0] == '(';
	}

	template<>
	inline auto TokenStream::is<Brackets::Square>() -> bool
	{
		token.expect("[");
		return !is_end() && token->code[0] == '[';
	}

	template<>
	inline auto TokenStream::is<Brackets::Curly>() -> bool
	{
		token.expect("{");
		return !is_end() && token->code[0] == '{';
	}

	template<>
	inline auto TokenStream::read<Brackets::Round>() -> token_tree::Tokens
	{
		if (!is<Brackets::Round>())
			throw error();

		auto tokens = token->subtokens;
		++token;
		return tokens;
	}

	template<>
	inline auto TokenStream::read<Brackets::Square>() -> token_tree::Tokens
	{
		if (!is<Brackets::Square>())
			throw error();

		auto tokens = token->subtokens;
		++token;
		return tokens;
	}

	template<>
	inline auto TokenStream::read<Brackets::Curly>() -> token_tree::Tokens
	{
		if (!is<Brackets::Curly>())
			throw error();

		auto tokens = token->subtokens;
		++token;
		return tokens;
	}
}
