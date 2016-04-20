#pragma once

#include "token_stream.hpp"

namespace pycxx
{
	template<>
	inline auto TokenStream::is<ast::Atom>() -> bool
	{
		return token->type == token_tree::Token::Atom;
	}

	template<>
	inline auto TokenStream::read<ast::Atom>() -> ast::Atom
	{
		if (!is<ast::Atom>())
			throw error();

		return {*token++};
	}

	template<>
	inline auto TokenStream::read<ast::Identifier>() -> ast::Identifier
	{
		return {read_label()};
	}
}
