#
# Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
.PHONY: all clean test example benchmarks
CXX?=clang++

all: test example benchmarks

benchmarks: benchmarks_simple benchmarks_complex

benchmarks_%:
	time $(CXX) benchmarks/$*/msm-lite.cpp -O2 -I include -I. -std=c++1y && ./a.out
	time $(CXX) benchmarks/$*/sc.cpp -O2 -I include -I. -std=c++1y && ./a.out
	time $(CXX) benchmarks/$*/euml.cpp -O2 -I include -I. -std=c++1y && ./a.out

test:
	$(CXX) test/ft.cpp -I include -I. -std=c++1y -Wall -Wextra -pedantic -fno-exceptions -Werror -pedantic-errors -include test/test.hpp && ./a.out

example:
	$(CXX) example/msm.cpp -I include -I ../boost/libs/di/include/ -I. -std=c++1y -Wall -Wextra -pedantic -fno-exceptions -Werror -pedantic-errors && ./a.out

clean:
	rm -f a.out
