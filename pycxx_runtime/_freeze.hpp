#pragma once

#include <utility>

//#include <iostream>
//#include "_type_name.hpp"

namespace pycxx_runtime
{
	namespace _freeze
	{
		//using _type_name::type_name;

		template <typename T>
		inline auto freeze(T&& v) -> T const&&
		{
			//std::cout << "f1 " << type_name<T>() << std::endl;
			return std::move(v);
		}

		template <typename T>
		inline auto freeze(const T&& v) -> T const&&
		{
			//std::cout << "f2 " << type_name<T>() << std::endl;
			return std::move(v);
		}

		template <typename T>
		inline auto freeze(T& v) -> T const &
		{
			//std::cout << "f3 " << type_name<T>() << std::endl;
			return v;
		}

		template <typename T>
		inline auto freeze(const T& v) -> T const &
		{
			//std::cout << "f4 " << type_name<T>() << std::endl;
			return v;
		}
	}
}
