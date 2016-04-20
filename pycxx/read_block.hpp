#pragma once

#include "token_stream.hpp"
#include "ast.hpp"
#include "parse.hpp"

namespace pycxx
{
	template<>
	inline auto TokenStream::is<ast::Block>() -> bool
	{
		return is(":");
	}

	template<>
	inline auto TokenStream::read<ast::Block>() -> ast::Block
	{
		match(":");

		return parse(block);
	}
}
