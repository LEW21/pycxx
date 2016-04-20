#pragma once

#include <string>
#include "token_tree.hpp"

namespace pycxx
{
	using std::string;

	auto tokenize(const string&) -> token_tree::Statements;
}
