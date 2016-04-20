#pragma once

#include "token_stream.hpp"

#include "read_atom.hpp"
#include "read_brackets.hpp"

namespace pycxx
{
	template<>
	inline auto TokenStream::read<ast::Expr>() -> ast::Expr
	{
		auto&& expr = [&]() -> ast::Expr
		{
			if (try_match("None"))
				return nullptr;

			if (try_match("..."))
				return ast::ellipsis{};

			if (try_match("True"))
				return true;

			if (try_match("False"))
				return false;

			if (is<ast::Atom>())
				return read<ast::Atom>();

			return read<ast::Identifier>();
		}();

		for (;;)
		{
			if (is<Brackets::Round>())
				expr = ast::Call{std::move(expr), parse_symbol<ast::Exprs>(read<Brackets::Round>())};
			else if (is<Brackets::Square>())
				expr = ast::Subscript{std::move(expr), parse_symbol<ast::Exprs>(read<Brackets::Square>())};
			else if (is("."))
				expr = ast::Property{std::move(expr), read_label()};
			else
				break;
		}

		return expr;
	}

	template<>
	inline auto TokenStream::read<ast::Exprs>() -> ast::Exprs
	{
		auto p = ast::Exprs{};
		while (!is_end())
		{
			p.emplace_back(read<ast::Expr>());
			if (!try_match(","))
				break;
		}
		return p;
	}

	template<>
	inline auto TokenStream::read<ast::ExprStatement>() -> ast::ExprStatement
	{
		return {read<ast::Expr>()};
	}
}
