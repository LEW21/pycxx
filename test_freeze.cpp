#include "pycxx_runtime.hpp"

#include <cassert>

using namespace pycxx_runtime;

void test_freeze()
{
	auto&& a = freeze(5); // rvalue
	auto&& b = freeze(a); // const-ref
	auto&& c = freeze("bbb");
	auto&& d = freeze((const char*)"bbb");

	auto&& e = 8;
	auto&& f = freeze(e); // lvalue

	auto&& g = freeze(freeze(5)); // const-rvalue

	assert(type_name<decltype(a)>() == "int const&&");
	assert(type_name<decltype(b)>() == "int const&");
	assert(type_name<decltype(c)>() == "char [4] const&");
	assert(type_name<decltype(d)>() == "char const* const&&");
	assert(type_name<decltype(e)>() == "int&&");
	assert(type_name<decltype(f)>() == "int const&");
	assert(type_name<decltype(g)>() == "int const&&");
}

int main()
{
	test_freeze();
}
