#
# Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
.PHONY: all doc clean test example
CXX?=clang++
CXXFLAGS:=-std=c++1y -Wall -Wextra -Werror -pedantic -pedantic-errors
VALGRIND:=valgrind --leak-check=full --error-exitcode=1
GCOV:=-fprofile-arcs -ftest-coverage
CLANG_FORMAT?=clang-format
CLANG_TIDY?=clang-tidy
PYTHON?=python
MKDOCS?=mkdocs
MKDOCS_THEME?=boost-experimental
MKDOCS_SITE?=site

all: test example

test: $(patsubst %.cpp, %.out, $(shell find test -maxdepth 1 -iname "*.cpp"))

test/%.out:
	$(CXX) test/$*.cpp $(CXXFLAGS) -fno-exceptions $($(COVERAGE)) -I include -I. -include test/test.hpp -o test/$*.out && $($(MEMCHECK)) test/$*.out

test/ft_except.out:
	$(CXX) test/ft_except.cpp $(CXXFLAGS) $($(COVERAGE)) -I include -I. -include test/test.hpp -o test/ft_except.out && $($(MEMCHECK)) test/ft_except.out

test/ft_thread.out:
	$(CXX) test/ft_thread.cpp $(CXXFLAGS) -DBOOST_MSM_LITE_THREAD_SAFE -lpthread -fsanitize=thread $($(COVERAGE)) -I include -I. -include test/test.hpp -o test/ft_thread.out && test/ft_thread.out
	$(CXX) test/ft_thread.cpp $(CXXFLAGS) -fno-exceptions -DBOOST_MSM_LITE_THREAD_SAFE -lpthread -fsanitize=thread $($(COVERAGE)) -I include -I. -include test/test.hpp -o test/ft_thread.out && test/ft_thread.out

test/ft_unit%.out:
	$(CXX) test/ft_unit$*.cpp $(CXXFLAGS) $($(COVERAGE)) -I include -c -o test/ft_unit$*.out

test/ft_units.out: test/ft_unit1.out test/ft_unit2.out
	$(CXX) test/ft_units.cpp $(CXXFLAGS) -fno-exceptions $($(COVERAGE)) -I include -I. -include test/test.hpp test/ft_unit1.out test/ft_unit2.out -o test/ft_units.out

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

doc: readme doc_$(MKDOCS_THEME)

doc_%:
	cd doc && $(MKDOCS) build --clean --theme-dir themes/$* --site-dir $(MKDOCS_SITE)

doc_boost-classic:
	cd doc && python themes/boost-classic/scripts/update_markdown.py . https://raw.githubusercontent.com/boost-experimental/msm-lite/master
	cd doc && $(MKDOCS) build --clean --theme-dir themes/boost-classic --site-dir $(MKDOCS_SITE)

readme:
	cd doc && $(PYTHON) scripts/update_readme_toc.py mkdocs.yml ../README.md http://boost-experimental.github.io/msm-lite

clean:
	find example test -iname "*.out" | xargs rm -f

