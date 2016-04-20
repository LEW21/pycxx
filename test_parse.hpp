#include "pycxx/tokenize.hpp"
#include "pycxx/token_stream.hpp"
#include <cassert>

using std::string;
using pycxx::tokenize;
using pycxx::TokenStream;

template <typename T>
void test_parse(string input, T output)
{
	auto&& block = tokenize(input);
	assert(block.size() == 1);

	auto val = TokenStream{block[0]}.read_end<T>();
	assert(val == output);
}
