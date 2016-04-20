## Pythonic syntax for C++

pycxx (temporary name) is a new programming language with syntax strongly based on Python, with good ideas copied from Rust. It is 100% compatible with C++ - pycxx modules are compiled to a clean and readable C++ code.

### Hello world

	def main() -> int:
		let s = "Hello world!"
		print(s)

### Build the example module

	# make && ./bin/compile aaa | tee aaa.cpp && g++ --std=c++14 -I. aaa.cpp main.cpp && ./a.out
