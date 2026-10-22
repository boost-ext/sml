#
# Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Thin wrapper around CMake/CTest.  All build logic lives in CMakeLists.txt.
#
# Usage:
#   make test                  # build and run tests (default: C++14)
#   make test CXXSTD=c++17     # build and run tests with C++17
#   make example               # build and run examples
#   make all                   # tests + examples
#   make style                 # clang-format check (requires configured build dir)
#   make static_analysis       # clang-tidy (requires configured build dir)
#   make clean                 # remove build directory
#
#   Memcheck:  make test MEMCHECK=VALGRIND
#   Coverage:  make test COVERAGE=GCOV
#
.PHONY: all test example style static_analysis clean

CXX    ?= g++
CXXSTD ?= c++14
_STD    = $(subst c++,,$(CXXSTD))
BDIR    = build-$(CXXSTD)

CMAKE   = cmake
CTEST   = ctest
NINJA  := $(shell command -v ninja 2>/dev/null)
ifdef NINJA
  GEN = -GNinja
else
  GEN =
endif

GCOV_FLAGS = -fprofile-arcs -ftest-coverage
ifdef MEMCHECK
  CTEST_FLAGS += -T memcheck
endif
ifdef COVERAGE
  CMAKE_EXTRA += -DCMAKE_CXX_FLAGS="$(GCOV_FLAGS)" -DCMAKE_EXE_LINKER_FLAGS="$(GCOV_FLAGS)"
endif

CMAKE_CONFIGURE = $(CMAKE) -B $(BDIR) -DCMAKE_CXX_COMPILER=$(CXX) \
                  -DCMAKE_CXX_STANDARD=$(_STD) $(GEN) $(CMAKE_EXTRA)

$(BDIR)/.configured:
	$(CMAKE_CONFIGURE) -DSML_BUILD_TESTS=ON -DSML_BUILD_EXAMPLES=ON .
	@touch $@

all: $(BDIR)/.configured
	$(CMAKE) --build $(BDIR)
	$(CTEST) --test-dir $(BDIR) --output-on-failure $(CTEST_FLAGS)

test: $(BDIR)/.configured
	$(CMAKE) --build $(BDIR)
	$(CTEST) --test-dir $(BDIR) --output-on-failure $(CTEST_FLAGS) -E "^example_"

example: $(BDIR)/.configured
	$(CMAKE) --build $(BDIR)
	$(CTEST) --test-dir $(BDIR) --output-on-failure $(CTEST_FLAGS) -R "^example_"

style: $(BDIR)/.configured
	$(CMAKE) --build $(BDIR) --target style

static_analysis: $(BDIR)/.configured
	$(CMAKE) --build $(BDIR) --target clang-tidy

clean:
	rm -rf $(BDIR)
