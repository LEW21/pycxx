#include <pycxx_runtime.hpp>

namespace aaa {

inline auto main() -> int
{
	auto&& s = pycxx_runtime::freeze("Hello world!");
	pycxx_runtime::print(s);
}

}
