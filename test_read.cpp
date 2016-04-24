#include "test_parse.hpp"
#include "pycxx/read_funcdecl.hpp"
#include "pycxx/read_letdecl.hpp"
#include "pycxx/read_loop.hpp"

using namespace pycxx::ast;

void test_func()
{
	test_parse("def x(typed: int, generic):\n\tpass",
		FuncDecl{
			false,
			"x",
			{{TypedPat{PatIdent{false, false, "typed"}, Expr{"int"_id}}}, {{PatIdent{false, false, "generic"}}}},
			nullopt,
			Block{Pass{}}
		}
	);
	test_parse("def mut x():\n\tpass\n\tpass",
		FuncDecl{
			true,
			"x",
			{},
			nullopt,
			Block{Pass{}, Pass{}}
		}
	);
}

void test_let()
{
	test_parse("let a",          LetDecl{{PatIdent{false, false, "a"}, nullopt}, nullopt});
	test_parse("let b: int = 5", LetDecl{{PatIdent{false, false, "b"}, Expr{"int"_id}}, 5_expr});
	test_parse("let c = 3",      LetDecl{{PatIdent{false, false, "c"}, nullopt}, 3_expr});
	test_parse("let (d, e)",     LetDecl{{PatTuple{{Pats{PatIdent{false, false, "d"}, PatIdent{false, false, "e"}}}}, nullopt}, nullopt});
	test_parse("let mut f",      LetDecl{{PatIdent{false, true, "f"}, nullopt}, nullopt});
}

void test_loops()
{
	test_parse("loop:\n\tpass", Loop{{Block{Pass{}}}});
	test_parse("for a in 6:\n\tpass", ForLoop{{PatIdent{false, false, "a"}, nullopt}, Expr{6_expr}, Block{Pass{}}});
}

int main()
{
	test_func();
	test_let();
	test_loops();
}
