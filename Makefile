CXX=clang++
CXXFLAGS=-Wall -Werror -Wextra -pedantic -Wno-char-subscripts -Wno-sign-compare -Wno-unknown-pragmas -Wno-error=unused-parameter -Wno-missing-field-initializers -g -std=c++14 -I. -fdiagnostics-color -Wl,-E

all: bin/compile

test: test_tokenize test_read_letdecl test_read_funcdecl test_freeze

test_tokenize: bin/test_tokenize
	bin/test_tokenize

bin/test_tokenize: test_tokenize.cpp pycxx/*.cpp pycxx/*.hpp pycxx_runtime/*.hpp *.hpp
	@test -d bin/ || mkdir -p bin/
	$(CXX) $(CXXFLAGS) test_tokenize.cpp pycxx/*.cpp -o bin/test_tokenize

test_read_letdecl: bin/test_read_letdecl
	bin/test_read_letdecl

bin/test_read_letdecl: test_read_letdecl.cpp pycxx/*.cpp pycxx/*.hpp pycxx_runtime/*.hpp *.hpp
	@test -d bin/ || mkdir -p bin/
	$(CXX) $(CXXFLAGS) test_read_letdecl.cpp pycxx/*.cpp -o bin/test_read_letdecl

test_read_funcdecl: bin/test_read_funcdecl
	bin/test_read_funcdecl

bin/test_read_funcdecl: test_read_funcdecl.cpp pycxx/*.cpp pycxx/*.hpp pycxx_runtime/*.hpp *.hpp
	@test -d bin/ || mkdir -p bin/
	$(CXX) $(CXXFLAGS) test_read_funcdecl.cpp pycxx/*.cpp -o bin/test_read_funcdecl

test_freeze: bin/test_freeze
	bin/test_freeze

bin/test_freeze: test_freeze.cpp pycxx_runtime/*.hpp *.hpp
	@test -d bin/ || mkdir -p bin/
	$(CXX) $(CXXFLAGS) test_freeze.cpp -o bin/test_freeze

bin/compile: compile.cpp pycxx/*.cpp pycxx/*.hpp pycxx_runtime/*.hpp *.hpp
	@test -d bin/ || mkdir -p bin/
	$(CXX) $(CXXFLAGS) compile.cpp pycxx/*.cpp -o bin/compile
