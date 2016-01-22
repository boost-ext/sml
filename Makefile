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
CLANG_FORMAT?=clang-format
CLANG_TIDY?=clang-tidy
MKDOCS?=mkdocs

all: test example pt

pt: pt_header pt_simple pt_composite pt_complex

pt_%:
	time $(CXX) test/pt/$*/msm-lite.cpp -O2 -s -I include -I test/pt -std=c++1y && ./a.out && ls -lh a.out
	time $(CXX) test/pt/$*/sc.cpp -ftemplate-depth=1024 -O2 -s -I include -I test/pt -std=c++1y && ./a.out && ls -lh a.out
	time $(CXX) test/pt/$*/euml.cpp -ftemplate-depth=1024 -O2 -s -I include -I test/pt -std=c++1y && ./a.out && ls -lh a.out

test: test_ut test_ft

test_%:
	$(CXX) test/$*.cpp $(CXXFLAGS) -I include -I. -include test/test.hpp -o $*.out && $($(MEMCHECK)) ./$*.out

example: $(patsubst %.cpp, example_%.cpp, $(shell find example -iname *.cpp -exec basename {} \;))

example_%:
	$(CXX) example/$* $(CXXFLAGS) -I include -o $*.out && $($(MEMCHECK)) ./$*.out

check: check_style check_static_analysis

check_style:
	@find include example test -iname "*.hpp" -or -iname "*.cpp" | xargs $(CLANG_FORMAT) -i
	@git diff include example test
	@exit `git ls-files -m include example test | wc -l`

check_static_analysis:
	$(CLANG_TIDY) -header-filter='msm' `find example test -type f -iname "*.cpp"` -- -std=c++1y -I include -I test -include test.hpp

doc:
	cd doc && $(MKDOCS) build

clean:
	@rm -f *.out

