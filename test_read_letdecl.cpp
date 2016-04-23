#include "test_parse.hpp"
#include "pycxx/read_letdecl.hpp"

using pycxx::ast::LetDecl;
using pycxx::ast::Expr;
using pycxx::ast::Identifier;
using pycxx::ast::PatIdent;
using pycxx::ast::PatTuple;
using pycxx::ast::Pats;
using pycxx::nullopt;
using pycxx::ast::operator ""_id;
using pycxx::ast::operator ""_expr;

void test_let()
{
	test_parse("let a",          LetDecl{{PatIdent{false, false, "a"}, nullopt}, nullopt});
	test_parse("let b: int = 5", LetDecl{{PatIdent{false, false, "b"}, Expr{"int"_id}}, 5_expr});
	test_parse("let c = 3",      LetDecl{{PatIdent{false, false, "c"}, nullopt}, 3_expr});
	test_parse("let (d, e)",     LetDecl{{PatTuple{{Pats{PatIdent{false, false, "d"}, PatIdent{false, false, "e"}}}}, nullopt}, nullopt});
	test_parse("let mut f",      LetDecl{{PatIdent{false, true, "f"}, nullopt}, nullopt});
}

int main()
{
	test_let();
}
