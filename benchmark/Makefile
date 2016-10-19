#
# Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
#
.PHONY: benchmark
CXX?=clang++

all: benchmark_header benchmark_simple benchmark_composite benchmark_complex

benchmark_%:
	time $(CXX) $*/sml.cpp -DCHECK_COMPILE_TIME -fno-exceptions -O2 -s -I ../include -I . -std=c++1y;
	@$(CXX) $*/sml.cpp -O2 -s -I ../include -I . -std=c++1y && ./a.out && ls -lh a.out
	time $(CXX) $*/sc.cpp -DCHECK_COMPILE_TIME -fno-exceptions -ftemplate-depth=1024 -O2 -s -I . -std=c++1y;
	@$(CXX) $*/sc.cpp -ftemplate-depth=1024 -O2 -s -I . -std=c++1y && ./a.out && ls -lh a.out
	time $(CXX) $*/euml.cpp -DCHECK_COMPILE_TIME -fno-exceptions -ftemplate-depth=1024 -O2 -s -lboost_system -I . -std=c++1y;
	@$(CXX) $*/euml.cpp -ftemplate-depth=1024 -O2 -s -lboost_system -I . -std=c++1y && ./a.out && ls -lh a.out
	time $(CXX) $*/euml2.cpp -DCHECK_COMPILE_TIME -fno-exceptions -O2 -s -I . -std=c++1y -lboost_system;
	@$(CXX) $*/euml2.cpp -O2 -s -I . -std=c++1y -lboost_system && ./a.out && ls -lh a.out
