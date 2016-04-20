#pragma once

#include <stdexcept>
#include <string>
#include <vector>

namespace pycxx
{
	namespace errors
	{
		using std::vector;
		using std::string;

		inline auto format_error(vector<string> expected, string got)
		{
			auto error = string{"Expected: "};

			bool first = true;
			for (const auto& e : expected)
			{
				if (!first)
					error += ", ";
				else
					first = false;

				error += e;
			}

			error += "\nGot: " + got;

			return error;
		}

		struct SyntaxError: public std::logic_error
		{
			using std::logic_error::logic_error;
			explicit SyntaxError(vector<string>&& e, const string& g): logic_error(format_error(e, g)), expected(std::move(e)), got(g) {}

			vector<string> expected;
			string got;
		};

		struct IndentationError: public SyntaxError
		{
			using SyntaxError::SyntaxError;
		};

		struct TabError: public IndentationError
		{
			using IndentationError::IndentationError;
		};
	}
}
