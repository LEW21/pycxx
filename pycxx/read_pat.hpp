#pragma once

#include "token_stream.hpp"

#include "read_vector.hpp"
#include "read_brackets.hpp"
#include "read_expr.hpp"

namespace pycxx
{
	template<>
	inline auto TokenStream::read<ast::PatIdent>() -> ast::PatIdent
	{
		return ast::PatIdent{try_match("&"), try_match("mut"), read_label()};
	}

	template<>
	inline auto TokenStream::is<ast::PatTuple>() -> bool
	{
		return is<Brackets::Round>();
	}

	template<>
	inline auto TokenStream::read<ast::PatTuple>() -> ast::PatTuple
	{
		return ast::PatTuple{parse_symbol<ast::Pats>(read<Brackets::Round>())};
	}

	template<>
	inline auto TokenStream::read<ast::Pat>() -> ast::Pat
	{
		if (is<ast::PatTuple>())
			return read<ast::PatTuple>();

		return read<ast::PatIdent>();
	}

	template<>
	inline auto TokenStream::read<ast::Pats>() -> ast::Pats
	{
		return read_vector<ast::Pat>(*this);
	}

	template<>
	inline auto TokenStream::read<ast::TypedPat>() -> ast::TypedPat
	{
		return ast::TypedPat{read<ast::Pat>(), try_match(":") ? optional<ast::Expr>{read<ast::Expr>()} : nullopt};
	}
}
