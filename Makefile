#
# Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
.PHONY: all clean test example benchmarks
CXX?=clang++
VALGRIND:=valgrind --leak-check=full --error-exitcode=1

all: test example benchmarks

benchmarks: benchmarks_header benchmarks_simple benchmarks_complex

benchmarks_%:
	time $(CXX) benchmarks/$*/msm-lite.cpp -O2 -I include -I benchmarks -std=c++1y && ./a.out
	time $(CXX) benchmarks/$*/sc.cpp -O2 -I include -I benchmarks -std=c++1y && ./a.out
	time $(CXX) benchmarks/$*/euml.cpp -O2 -I include -I benchmarks -std=c++1y && ./a.out

test: test_ut test_ft

test_%:
	$(CXX) test/$*.cpp -I include -I. -std=c++1y -Wall -Wextra -pedantic -fno-exceptions -Werror -pedantic-errors -include test/test.hpp -o $*.out && $($(MEMCHECK)) ./$*.out

example: $(shell find example -iname *.cpp -printf "example_%f\n")

example_%:
	$(CXX) example/$* -I include -std=c++1y -Wall -Wextra -pedantic -fno-exceptions -Werror -pedantic-errors -o $*.out && $($(MEMCHECK))./$*.out

clean:
	@rm -f *.out
