#pragma once

#include "token_tree.hpp"
#include "ast.hpp"

namespace pycxx
{
	auto parse(token_tree::Statements) -> ast::Block;
}
