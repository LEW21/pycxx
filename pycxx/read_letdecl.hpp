#pragma once

#include "token_stream.hpp"

#include "read_pat.hpp"
#include "read_expr.hpp"

namespace pycxx
{
	template<>
	inline auto TokenStream::is<ast::LetDecl>() -> bool
	{
		return is("let");
	}

	// LetDecl := "let" "mut"? (IDENT ",")* IDENT (":" expr)? ("=" expr)?
	// may want to change to:
	// LetDecl := "let" "mut"? (IDENT | ("(" (IDENT ",")* IDENT ")")) (":" expr)? ("=" expr)?
	// because in parameters : and = apply to the one directly before, and here they apply to everything
	// or even to:
	// Let := "mut"? (IDENT | ("(" (IDENT ",")* IDENT ")")) (":" expr)? ("=" expr)?
	// LetDecl := "let" (Let ",")* Let
	// however there is the question how to interpret:
	// let x = 5, y = 7
	// as (x = 5), (y = 7) or x = (5, y = 7)
	template<>
	inline auto TokenStream::read<ast::LetDecl>() -> ast::LetDecl
	{
		match("let");

		auto&& l = ast::LetDecl{read<ast::TypedPat>()};

		if (try_match("="))
			l.value = read<ast::Expr>();

		return l;
	}
}
