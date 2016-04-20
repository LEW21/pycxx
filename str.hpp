#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

namespace str
{
	using std::vector;
	using std::string;

	inline auto split(string self, char sep) -> vector<string>
	{
		auto&& ret = vector<string>{};
		boost::algorithm::split(ret, self, [sep](char c){return c == sep;});
		return ret;
	}

	template <typename V>
	inline auto join(string self, V&& list) -> string
	{
		auto&& s = string{};
		auto&& first = true;
		for (auto&& v : std::forward<V>(list))
		{
			if (first)
				first = false;
			else
				s += self;
			s += v;
		}
		return s;
	}

	inline auto replace(string self, char find, string replace) -> string
	{
		return join(replace, split(self, find));
	}

	inline auto count(string self, char sub) -> int
	{
		auto&& n = 0;
		for (auto&& c : self)
			if (c == sub)
				n += 1;
		return n;
	}

	inline auto startswith(string self, string prefix) -> bool
	{
		return boost::algorithm::starts_with(self, prefix);
	}

	inline auto endswith(string self, string suffix) -> bool
	{
		return boost::algorithm::ends_with(self, suffix);
	}

	// [start:end]
	inline auto slice(string self, int start = 0, int end = -1) -> string
	{
		if (end == -1)
			return self.substr(start);
		return self.substr(start, end-start);
	}
}
