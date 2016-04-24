#pragma once

#include "token_stream.hpp"
#include "ast.hpp"

#include "read_pass.hpp"
#include "read_letdecl.hpp"
#include "read_funcdecl.hpp"
#include "read_loop.hpp"

namespace pycxx
{
	template<>
	inline auto TokenStream::read<ast::Statement>() -> ast::Statement
	{
		if (is<ast::Pass>())
			return read<ast::Pass>();

		if (is<ast::LetDecl>())
			return read<ast::LetDecl>();

		if (is<ast::FuncDecl>())
			return read<ast::FuncDecl>();

		if (is<ast::Loop>())
			return read<ast::Loop>();

		if (is<ast::ForLoop>())
			return read<ast::ForLoop>();

		return read<ast::ExprStatement>();
	}
}
