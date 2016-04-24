#pragma once

#include "token_stream.hpp"

#include "read_pat.hpp"
#include "read_expr.hpp"

namespace pycxx
{
	template<>
	inline auto TokenStream::is<ast::Loop>() -> bool
	{
		return is("loop");
	}

	template<>
	inline auto TokenStream::read<ast::Loop>() -> ast::Loop
	{
		match("loop");
		return ast::Loop{read<ast::Block>()};
	}

	template<>
	inline auto TokenStream::is<ast::ForLoop>() -> bool
	{
		return is("for");
	}

	template<>
	inline auto TokenStream::read<ast::ForLoop>() -> ast::ForLoop
	{
		return ast::ForLoop{(match("for"), read<ast::TypedPat>()), (match("in"), read<ast::Expr>()), read<ast::Block>()};
	}
}
