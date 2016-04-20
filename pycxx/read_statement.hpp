#pragma once

#include "token_stream.hpp"
#include "ast.hpp"

#include "read_pass.hpp"
#include "read_letdecl.hpp"
#include "read_funcdecl.hpp"

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

		return read<ast::ExprStatement>();
	}
}
