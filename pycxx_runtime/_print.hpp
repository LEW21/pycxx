#pragma once

#include <iostream>

namespace pycxx_runtime
{
	namespace _print
	{
		inline void print()
		{
			std::cout << std::endl;
		}

		template <typename First, typename... Rest>
		inline void print(First&& first, Rest&&... rest)
		{
			std::cout << first;
			print(rest...);
		}
	}
}
