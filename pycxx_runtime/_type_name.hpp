#pragma once

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>

namespace pycxx_runtime
{
	namespace _type_name
	{
		using std::string;
		using std::remove_reference_t;
		using std::unique_ptr;
		using std::free;

		template <class T>
		inline auto type_name() -> string
		{
			using TR = remove_reference_t<T>;

			auto n = typeid(TR).name();

			#ifndef _MSC_VER
			auto own = unique_ptr<char, void(*)(void*)>{abi::__cxa_demangle(n, nullptr, nullptr, nullptr), free};
			if (own)
				n = own.get();
			#endif

			auto r = string{n};

			if (std::is_const<TR>::value)
				r += " const";
			if (std::is_volatile<TR>::value)
				r += " volatile";
			if (std::is_lvalue_reference<T>::value)
				r += "&";
			else if (std::is_rvalue_reference<T>::value)
				r += "&&";
			return r;
		}
	}
}
