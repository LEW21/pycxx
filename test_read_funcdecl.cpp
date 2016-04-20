#include "test_parse.hpp"
#include "pycxx/read_funcdecl.hpp"

using std::string;
using pycxx::ast::FuncDecl;
using pycxx::ast::Pass;
using pycxx::ast::Block;
using pycxx::ast::Expr;
using pycxx::nullopt;
using pycxx::ast::operator ""_id;
using pycxx::ast::operator ""_expr;

void test_func()
{
	test_parse("def x(typed: int, generic):\n\tpass",
		FuncDecl{
			false,
			"x",
			{{"typed", Expr{"int"_id}}, {"generic"}},
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

int main()
{
	test_func();
}
