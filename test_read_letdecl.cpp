#include "test_parse.hpp"
#include "pycxx/read_letdecl.hpp"

using pycxx::ast::LetDecl;
using pycxx::ast::Expr;
using pycxx::ast::Identifier;
using pycxx::nullopt;
using pycxx::ast::operator ""_id;
using pycxx::ast::operator ""_expr;

void test_let()
{
	test_parse("let a",          LetDecl{false, {"a"},      nullopt,        nullopt});
	test_parse("let b: int = 5", LetDecl{false, {"b"},      Expr{"int"_id}, 5_expr});
	test_parse("let c = 3",      LetDecl{false, {"c"},      nullopt,        3_expr});
	test_parse("let d, e",       LetDecl{false, {"d", "e"}, nullopt,        nullopt});
	test_parse("let mut f",      LetDecl{true,  {"f"},      nullopt,        nullopt});
}

int main()
{
	test_let();
}
