#
# Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
.PHONY: all doc clean test example
CXX?=clang++
CXXSTD?=c++14

ifneq (, $(findstring clang++, $(CXX)))
	CXXFLAGS+=-std=$(CXXSTD) -Wall -Wextra -Werror -pedantic -pedantic-errors -I include -I .
	INCLUDE_TEST=-include test/common/test.hpp
	DISABLE_EXCEPTIONS=-fno-exceptions
else ifneq (, $(findstring g++, $(CXX)))
	CXXFLAGS+=-std=$(CXXSTD) -Wall -Wextra -Werror -pedantic -pedantic-errors -I include -I .
	INCLUDE_TEST=-include test/common/test.hpp
	DISABLE_EXCEPTIONS=-fno-exceptions
else
	CXXFLAGS+=-EHsc -W4 -WX -I include -I .
	INCLUDE_TEST=-FI test/common/test.hpp
	DISABLE_EXCEPTIONS=
endif
VALGRIND:=valgrind --leak-check=full --error-exitcode=1
DRMEMORY:=drmemory -light -batch -exit_code_if_errors 1 --
GCOV:=-fprofile-arcs -ftest-coverage
CLANG_FORMAT?=clang-format
CLANG_TIDY?=clang-tidy
PYTHON?=python2
MKDOCS?=mkdocs
MKDOCS_THEME?=boost-experimental
MKDOCS_SITE?=site

all: test example

pph:
	@tools/pph.sh

check: style

test: $(patsubst %.cpp, %.out, $(wildcard test/ft/*.cpp test/ft/errors/*.cpp test/ut/*.cpp test/unit/*.cpp))

test/ut/%.out:
	$(CXX) test/ut/$*.cpp $(CXXFLAGS) $(DISABLE_EXCEPTIONS) $($(COVERAGE)) $(INCLUDE_TEST) -o test/ut/$*.out && $($(MEMCHECK)) test/ut/$*.out

test/ft/errors/%.out:
	$(CXX) test/ft/errors/$*.cpp $(CXXFLAGS) -I include 2> /dev/null || [ $$? -ne 0 ]

test/ft/%.out:
	$(CXX) test/ft/$*.cpp $(CXXFLAGS) $(DISABLE_EXCEPTIONS) $($(COVERAGE)) $(INCLUDE_TEST) -o test/ft/$*.out && $($(MEMCHECK)) test/ft/$*.out

test/ft/sizeof.out:
	$(CXX) test/ft/sizeof.cpp $(CXXFLAGS) -ftemplate-depth=1024 $(DISABLE_EXCEPTIONS) $($(COVERAGE)) $(INCLUDE_TEST) -o test/ft/sizeof.out && $($(MEMCHECK)) test/ft/sizeof.out

test/ft/state_machine.out:
	$(CXX) test/ft/state_machine.cpp $(CXXFLAGS) -ftemplate-depth=1024 $(DISABLE_EXCEPTIONS) $($(COVERAGE)) $(INCLUDE_TEST) -o test/ft/state_machine.out && $($(MEMCHECK)) test/ft/state_machine.out

test/ft/exceptions.out:
	$(CXX) test/ft/exceptions.cpp $(CXXFLAGS) $($(COVERAGE)) $(INCLUDE_TEST) -o test/ft/exceptions.out && $($(MEMCHECK)) test/ft/exceptions.out

test/ft/policies_thread_safe.out: #-fsanitize=thread
	$(CXX) test/ft/policies_thread_safe.cpp $(CXXFLAGS) $(DISABLE_EXCEPTIONS) -lpthread $($(COVERAGE)) $(INCLUDE_TEST) -o test/ft/policies_thread_safe.out && $($(MEMCHECK)) test/ft/policies_thread_safe.out

test/ft/constexpr.out:
	$(CXX) test/ft/constexpr.cpp $(CXXFLAGS) -ftemplate-depth=1024 $(DISABLE_EXCEPTIONS) $($(COVERAGE)) $(INCLUDE_TEST) -o test/ft/constexpr.out && $($(MEMCHECK)) test/ft/constexpr.out

test/unit/%.out:
	$(CXX) test/unit/unit1.cpp test/unit/unit2.cpp test/unit/units.cpp $(CXXFLAGS) $(DISABLE_EXCEPTIONS) $($(COVERAGE)) -o test/unit/units.out

example: $(patsubst %.cpp, %.out, $(wildcard example/*.cpp))

example/%.out:
	$(CXX) example/$*.cpp $(CXXFLAGS) -o example/$*.out && $($(MEMCHECK)) example/$*.out

style:
	@find include example test -iname "*.hpp" -or -iname "*.cpp" | xargs $(CLANG_FORMAT) -i
	@git diff include example test
	@exit `git ls-files -m include example test | wc -l`

pph_check: pph
	@git diff include/boost/sml.hpp
	@git diff --quiet include/boost/sml.hpp

static_analysis:
	$(CLANG_TIDY) $(wildcard test/ft/*.cpp test/ut/*.cpp) -- -std=c++1y -I include -I test -FI common/test.hpp

doc: readme doc_$(MKDOCS_THEME)

doc_%:
	cd doc && $(MKDOCS) build --clean --theme-dir themes/$* --site-dir $(MKDOCS_SITE)

doc_boost-classic:
	cd doc && $(PYTHON) themes/boost-classic/scripts/update_markdown.py . https://raw.githubusercontent.com/boost-experimental/sml/master
	cd doc && $(MKDOCS) build --clean --theme-dir themes/boost-classic --site-dir $(MKDOCS_SITE)

readme:
	cd doc && $(PYTHON) scripts/update_readme_toc.py mkdocs.yml ../README.md http://boost-experimental.github.io/sml

clean:
	find example test -iname "*.out" -or -iname "*.obj" | xargs rm -f

