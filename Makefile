#
# Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
.PHONY: all doc clean test example
CXX?=clang++
ifneq (, $(findstring clang++, $(CXX)))
	CXXFLAGS?=-std=c++1y -Wall -Wextra -Werror -pedantic -pedantic-errors -I include -I .
	INCLUDE_TEST=-include test/common/test.hpp
else ifneq (, $(findstring g++, $(CXX)))
	CXXFLAGS?=-std=c++1y -Wall -Wextra -Werror -pedantic -pedantic-errors -I include -I .
	INCLUDE_TEST=-include test/common/test.hpp
else
	CXXFLAGS?=-EHsc -W3 -I include -I .
	INCLUDE_TEST=-FI test/common/test.hpp
endif
VALGRIND:=valgrind --leak-check=full --error-exitcode=1
DRMEMORY:=drmemory -light -batch -exit_code_if_errors 1 --
GCOV:=-fprofile-arcs -ftest-coverage
CLANG_FORMAT?=clang-format
CLANG_TIDY?=clang-tidy
PYTHON?=python
MKDOCS?=mkdocs
MKDOCS_THEME?=boost-experimental
MKDOCS_SITE?=site

all: test example

pph:
	@tools/pph.sh

check: style

test: $(patsubst %.cpp, %.out, $(wildcard test/ft/*.cpp test/ut/*.cpp))

test/ft/%.out:
	$(CXX) test/ft/$*.cpp $(CXXFLAGS) -fno-exceptions $($(COVERAGE)) $(INCLUDE_TEST) -o test/ft/$*.out	&& $($(MEMCHECK)) test/ft/$*.out

test/ut/%.out:
	$(CXX) test/ut/$*.cpp $(CXXFLAGS) -fno-exceptions $($(COVERAGE)) $(INCLUDE_TEST) -o test/ut/$*.out	&& $($(MEMCHECK)) test/ut/$*.out

test/ft/sizeof.out:
	$(CXX) test/ft/sizeof.cpp $(CXXFLAGS) -ftemplate-depth=1024 -fno-exceptions $($(COVERAGE)) $(INCLUDE_TEST) -o test/ft/sizeof.out && $($(MEMCHECK)) test/ft/sizeof.out

test/ft/state_machine.out:
	$(CXX) test/ft/state_machine.cpp $(CXXFLAGS) -ftemplate-depth=1024 -fno-exceptions $($(COVERAGE)) $(INCLUDE_TEST) -o test/ft/state_machine.out && $($(MEMCHECK)) test/ft/state_machine.out

test/ft/exceptions.out:
	$(CXX) test/ft/exceptions.cpp $(CXXFLAGS) $($(COVERAGE)) $(INCLUDE_TEST) -o test/ft/exceptions.out && $($(MEMCHECK)) test/ft/exceptions.out

test/ft/policies_thread_safe.out: #-fsanitize=thread
	$(CXX) test/ft/policies_thread_safe.cpp $(CXXFLAGS) -fno-exceptions -lpthread $($(COVERAGE)) $(INCLUDE_TEST) -o test/ft/policies_thread_safe.out && $($(MEMCHECK)) test/ft/policies_thread_safe.out

test/ft/unit1.out:
	$(CXX) test/ft/unit1.cpp $(CXXFLAGS) $($(COVERAGE)) -c -o test/ft/unit1.out

test/ft/unit2.out:
	$(CXX) test/ft/unit2.cpp $(CXXFLAGS) $($(COVERAGE)) -c -o test/ft/unit2.out

test/ft/units.out: test/ft/unit1.out test/ft/unit2.out
	$(CXX) test/ft/units.cpp $(CXXFLAGS) -fno-exceptions $($(COVERAGE)) $(INCLUDE_TEST) test/ft/unit1.out test/ft/unit2.out -o test/ft/units.out

example: $(patsubst %.cpp, %.out, $(wildcard example/*.cpp example/errors/*.cpp))

example/errors/%.out:
	$(CXX) example/errors/$*.cpp $(CXXFLAGS) -I include 2> /dev/null || [ $$? -ne 0 ]

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
	cd doc && python themes/boost-classic/scripts/update_markdown.py . https://raw.githubusercontent.com/boost-experimental/sml/master
	cd doc && $(MKDOCS) build --clean --theme-dir themes/boost-classic --site-dir $(MKDOCS_SITE)

readme:
	cd doc && $(PYTHON) scripts/update_readme_toc.py mkdocs.yml ../README.md http://boost-experimental.github.io/sml

clean:
	find example test -iname "*.out" -or -iname "*.obj" | xargs rm -f

