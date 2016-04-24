#pragma once

#include "token_stream.hpp"

namespace pycxx
{
	template<typename T>
	inline auto read_vector(TokenStream& t) -> vector<T>
	{
		auto&& p = vector<T>{};
		while (!t.is_end())
		{
			p.emplace_back(t.read<T>());
			if (!t.try_match(","))
				break;
		}
		return p;
	}
}
