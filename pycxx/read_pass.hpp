#pragma once

#include "token_stream.hpp"

namespace pycxx
{
	template<>
	inline auto TokenStream::is<ast::Pass>() -> bool
	{
		return is("pass");
	}

	// Pass := "pass"
	template<>
	inline auto TokenStream::read<ast::Pass>() -> ast::Pass
	{
		match("pass");
		return ast::Pass{};
	}
}
