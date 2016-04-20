#pragma once

#include "token_stream.hpp"

#include "read_brackets.hpp"
#include "read_block.hpp"
#include "read_expr.hpp"

namespace pycxx
{
	template<>
	inline auto TokenStream::read<ast::ParamDecl>() -> ast::ParamDecl
	{
		auto p = ast::ParamDecl{};
		p.name = read_label();

		if (try_match(":"))
			p.type = read<ast::Expr>();

		if (try_match("="))
			p.default_value = read<ast::Expr>();

		return p;
	}

	template<>
	inline auto TokenStream::read<ast::ParamDecls>() -> ast::ParamDecls
	{
		auto p = ast::ParamDecls{};
		do
		{
			p.emplace_back(read<ast::ParamDecl>());
		} while (try_match(","));
		return p;
	}

	template<>
	inline auto TokenStream::is<ast::FuncDecl>() -> bool
	{
		return is("fn") || is("def");
	}

	template<>
	inline auto TokenStream::read<ast::FuncDecl>() -> ast::FuncDecl
	{
		if (!try_match("fn"))
			match("def");

		auto f = ast::FuncDecl{};
		f.is_mutable = try_match("mut");
		f.name = read_label();

		auto params = read<Brackets::Round>();
		if (params.size())
			f.params = parse_symbol<ast::ParamDecls>(params);

		if (try_match("->"))
			f.return_type = read<ast::Expr>();

		f.code = read<ast::Block>();

		return f;
	}
}
