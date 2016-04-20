#include "parse.hpp"

#include "read_statement.hpp"

namespace pycxx
{
	auto parse(token_tree::Statements statements) -> ast::Block
	{
		auto b = ast::Block{};
		for (auto&& s : statements)
			b.emplace_back(TokenStream{s}.read_end<ast::Statement>());
		return b;
	}
}
