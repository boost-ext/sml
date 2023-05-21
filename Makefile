#
# Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
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
PYTHON?=python3
MKDOCS?=mkdocs
MKDOCS_THEME?=boost-modern
MKDOCS_SITE?=site
PLANTCXX:=$(subst -std=c++14,-std=c++17,$(CXXFLAGS))

all: test example

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

example/plant_uml.out:
	# try to compile with C++17, if it didn't work C++14 instead
	$(CXX) example/plant_uml.cpp $(PLANTCXX) -o $@ || true
	@if [ ! -e "$@" ]; then\
		$(CXX) example/plant_uml.cpp $(CXXFLAGS) -o $@;\
	fi
	$($(MEMCHECK)) $@

style:
	@find include example test -iname "*.hpp" -or -iname "*.cpp" | xargs $(CLANG_FORMAT) -i
	@git diff include example test
	@exit `git ls-files -m include example test | wc -l`

static_analysis:
	$(CLANG_TIDY) $(wildcard test/ft/*.cpp test/ut/*.cpp) -- -std=c++1y -I include -I test -FI common/test.hpp

doc: readme doc_$(MKDOCS_THEME)

doc_boost-modern:
	MKDOCS_THEME_DIR='doc/themes/boost-modern' $(MKDOCS) build --quiet --config-file .$(MKDOCS).yml --clean --site-dir $(MKDOCS_SITE)

doc_boost-classic:
	$(PYTHON) doc/themes/boost-classic/scripts/update_markdown.py . https://raw.githubusercontent.com/boost-ext/sml/master
	MKDOCS_THEME_DIR='doc/themes/boost-classic' $(MKDOCS) build --quiet --config-file .$(MKDOCS).yml --clean --site-dir $(MKDOCS_SITE)

readme:
	$(PYTHON) doc/scripts/update_readme_toc.py doc .$(MKDOCS).yml README.md https://boost-ext.github.io/sml

clean:
	find example test -iname "*.out" -or -iname "*.obj" | xargs rm -f
