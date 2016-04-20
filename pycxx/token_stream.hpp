#pragma once

#include "errors.hpp"
#include "token_tree.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <boost/optional.hpp>

namespace pycxx
{
	using std::move;
	using std::string;
	using std::vector;
	using boost::optional;
	/*constexpr*/ const auto nullopt = boost::none;

	inline bool iequals(const string& a, const string& b)
	{
		if (b.size() != a.size())
			return false;
		for (size_t i = 0; i < a.size(); ++i)
			if (tolower(a[i]) != tolower(b[i]))
				return false;
			return true;
	}

	template <typename T>
	struct range
	{
		T i;
		T end;

		range(T i, T end): i(i), end(end) {}

		auto operator++() -> range& { ++i; return *this; }
		auto operator++(int) -> range { auto x = *this; ++i; return x; }
		auto operator*() { return *i; }
		auto operator*() const { return *i; }
		auto operator->() { return i.operator->(); }
		auto operator->() const { return i.operator->(); }

		auto operator!() const { return i == end; }
		operator bool() const { return !!(*this); }
	};

	template <typename T>
	struct debug_range
	{
		T i;
		std::vector<std::string> expected;

		template <typename... Arg>
		debug_range(Arg... arg): i(std::forward<Arg>(arg)...) {}

		auto expect(std::string x) {expected.push_back(x);}

		auto operator++() -> debug_range& { ++i; expected = {}; return *this; }
		auto operator++(int) -> debug_range { auto x = *this; ++i; expected = {}; return x; }
		auto operator*() { return *i; }
		auto operator*() const { return *i; }
		auto operator->() { return i.operator->(); }
		auto operator->() const { return i.operator->(); }

		auto operator!() const { return !i; }
		operator bool() const { return !!(*this); }
	};

	template <typename T>
	struct ParsingResultT { using type = T; };

	template <typename T>
	using ParsingResult = typename ParsingResultT<T>::type;

	struct TokenStream
	{
		debug_range<range<token_tree::Tokens::const_iterator>> token;
		token_tree::Statements block;

		TokenStream(const token_tree::Tokens& t): token{std::cbegin(t), std::cend(t)} {}
		TokenStream(const token_tree::Statement& s): token{std::cbegin(s.tokens), std::cend(s.tokens)}, block{s.block} {}

		auto error()
		{
			return errors::SyntaxError{std::move(token.expected), token ? string(*token) : "end-of-input"};
		}

		auto is_end()
		{
			return !token;
		}

		auto is(const char* code)
		{
			token.expect(code);
			return !is_end() && iequals(*token, code);
		}

		auto match_end()
		{
			token.expect("end");
			if (!is_end())
				throw error();
		}

		auto match(const char* code)
		{
			if (!is(code))
				throw error();
			++token;
		}

		auto try_match(const char* code) -> bool
		{
			if (!is(code))
				return false;
			++token;
			return true;
		}

		auto read_label() -> string
		{
			token.expect("label");
			if (is_end())
				throw error();
			return *(token++);
		}

		template <typename T>
		auto is() -> bool;

		template <typename T>
		auto read() -> ParsingResult<T>;

		template <typename T>
		auto try_read() -> optional<T>
		{
			if (is<T>())
				return read<T>();
			else
				return nullopt;
		}

		template <typename T>
		auto read_end() -> T
		{
			auto x = read<T>();
			match_end();
			return x;
		}
	};

	template <typename T>
	auto parse_symbol(const token_tree::Tokens& t) { return TokenStream{t}.read_end<T>(); }

	template <typename T>
	auto parse_symbol(const token_tree::Statement& s) { return TokenStream{s}.read_end<T>(); }
}
