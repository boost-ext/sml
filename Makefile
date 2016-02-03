#
# Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
.PHONY: all doc clean test example pt
CXX?=clang++
CXXFLAGS:=-std=c++1y -Wall -Wextra -Werror -pedantic -pedantic-errors -fno-exceptions
VALGRIND:=valgrind --leak-check=full --error-exitcode=1
GCOV:=-fprofile-arcs -ftest-coverage
CLANG_FORMAT?=clang-format
CLANG_TIDY?=clang-tidy
PYTHON?=python
MKDOCS?=mkdocs

all: test example pt

pt: pt_header pt_simple pt_composite pt_complex

pt_%:
	time $(CXX) test/pt/$*/msm-lite.cpp -O2 -s -I include -I test/pt -std=c++1y && ./a.out && ls -lh a.out
	time $(CXX) test/pt/$*/sc.cpp -ftemplate-depth=1024 -O2 -s -I include -I test/pt -std=c++1y && ./a.out && ls -lh a.out
	time $(CXX) test/pt/$*/euml.cpp -ftemplate-depth=1024 -O2 -s -I include -lboost_system -I test/pt -std=c++1y && ./a.out && ls -lh a.out
	time $(CXX) test/pt/$*/euml2.cpp -O2 -s -I include -I test/pt -std=c++1y -lboost_system && ./a.out && ls -lh a.out

test: $(patsubst %.cpp, %.out, $(shell find test -maxdepth 1 -iname "*.cpp"))

test/%.out:
	$(CXX) test/$*.cpp $(CXXFLAGS) $($(COVERAGE)) -I include -I. -include test/test.hpp -o test/$*.out && $($(MEMCHECK)) test/$*.out

example: $(patsubst %.cpp, %.out, $(shell find example -iname "*.cpp"))

example/errors/%.out:
	$(CXX) example/errors/$*.cpp $(CXXFLAGS) -I include || [ $$? -ne 0 ]

example/%.out:
	$(CXX) example/$*.cpp $(CXXFLAGS) -I include -o example/$*.out && $($(MEMCHECK)) example/$*.out

style_check:
	@find include example test -iname "*.hpp" -or -iname "*.cpp" | xargs $(CLANG_FORMAT) -i
	@git diff include example test
	@exit `git ls-files -m include example test | wc -l`

static_check:
	$(CLANG_TIDY) test/ut.cpp test/ft.cpp -- -std=c++1y -I include -I test -include test.hpp

doc:
	cd doc && $(MKDOCS) build --clean && $(PYTHON) boost/scripts/update_readme_toc.py mkdocs.yml ../README.md http://boost-experimental.github.io/msm-lite

clean:
	find example test -iname "*.out" | xargs rm -f

