SHELL := /bin/bash

#
# Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
#
.PHONY: benchmark
CXX?=clang++
ifneq (, $(findstring clang++, $(CXX)))
	CXXFLAGS?=-std=c++20 -fno-exceptions -O3 -s -I ../include -I .
else ifneq (, $(findstring g++, $(CXX)))
	CXXFLAGS?=-std=c++20 -fno-exceptions -O3 -s -I ../include -I .
else
	CXXFLAGS?=-nologo -EHsc -Ox -W3 -I include -I .
endif

all: benchmark_header benchmark_simple benchmark_composite benchmark_complex

benchmark_%:
	time $(CXX) $*/sml.lite.cpp $(CXXFLAGS) -DCHECK_COMPILE_TIME
	@$(CXX) $*/sml.lite.cpp $(CXXFLAGS) && ./a.out && ls -lh a.out
	time $(CXX) $*/switch.cpp $(CXXFLAGS) -DCHECK_COMPILE_TIME
	@$(CXX) $*/switch.cpp $(CXXFLAGS) && ./a.out && ls -lh a.out
	time $(CXX) $*/sml.cpp $(CXXFLAGS) -DCHECK_COMPILE_TIME
	@$(CXX) $*/sml.cpp $(CXXFLAGS) && ./a.out && ls -lh a.out
	time $(CXX) $*/variant.cpp $(CXXFLAGS) -std=c++1z -DCHECK_COMPILE_TIME
	@$(CXX) $*/variant.cpp $(CXXFLAGS) -std=c++1z && ./a.out && ls -lh a.out
	time $(CXX) $*/sc.cpp $(CXXFLAGS) -DCHECK_COMPILE_TIME -ftemplate-depth=1024
	@$(CXX) $*/sc.cpp $(CXXFLAGS) -ftemplate-depth=1024 && ./a.out && ls -lh a.out
	time $(CXX) $*/euml.cpp $(CXXFLAGS) -DCHECK_COMPILE_TIME -ftemplate-depth=1024 -lboost_system
	@$(CXX) $*/euml.cpp $(CXXFLAGS) -ftemplate-depth=1024 -lboost_system && ./a.out && ls -lh a.out
	time $(CXX) $*/euml2.cpp $(CXXFLAGS) -DCHECK_COMPILE_TIME -lboost_system
	@$(CXX) $*/euml2.cpp $(CXXFLAGS)-lboost_system && ./a.out && ls -lh a.out
