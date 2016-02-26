#
# Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
.PHONY: all doc clean test example pt
CXX?=clang++
CXXFLAGS:=-std=c++1y -Wall -Wextra -Werror -pedantic -pedantic-errors
VALGRIND:=valgrind --leak-check=full --error-exitcode=1
GCOV:=-fprofile-arcs -ftest-coverage
CLANG_FORMAT?=clang-format
CLANG_TIDY?=clang-tidy
PYTHON?=python
MKDOCS?=mkdocs
MKDOCS_THEME?=themes/boost-experimental
MKDOCS_SITE?=site

all: test example pt

pt: pt_header pt_simple pt_composite pt_complex

pt_%:
	time $(CXX) test/pt/$*/msm-lite.cpp -DCHECK_COMPILE_TIME -fno-exceptions -O2 -s -I include -I test/pt -std=c++1y;
	@$(CXX) test/pt/$*/msm-lite.cpp -O2 -s -I include -I test/pt -std=c++1y && ./a.out && ls -lh a.out
	time $(CXX) test/pt/$*/sc.cpp -DCHECK_COMPILE_TIME -fno-exceptions -ftemplate-depth=1024 -O2 -s -I include -I test/pt -std=c++1y;
	@$(CXX) test/pt/$*/sc.cpp -ftemplate-depth=1024 -O2 -s -I include -I test/pt -std=c++1y && ./a.out && ls -lh a.out
	time $(CXX) test/pt/$*/euml.cpp -DCHECK_COMPILE_TIME -fno-exceptions -ftemplate-depth=1024 -O2 -s -I include -lboost_system -I test/pt -std=c++1y;
	@$(CXX) test/pt/$*/euml.cpp -ftemplate-depth=1024 -O2 -s -I include -lboost_system -I test/pt -std=c++1y && ./a.out && ls -lh a.out
	time $(CXX) test/pt/$*/euml2.cpp -DCHECK_COMPILE_TIME -fno-exceptions -O2 -s -I include -I test/pt -std=c++1y -lboost_system;
	@$(CXX) test/pt/$*/euml2.cpp -O2 -s -I include -I test/pt -std=c++1y -lboost_system && ./a.out && ls -lh a.out

test: $(patsubst %.cpp, %.out, $(shell find test -maxdepth 1 -iname "*.cpp"))

test/%.out:
	$(CXX) test/$*.cpp $(CXXFLAGS) -fno-exceptions $($(COVERAGE)) -I include -I. -include test/test.hpp -o test/$*.out && $($(MEMCHECK)) test/$*.out

test/ft_except.out:
	$(CXX) test/ft_except.cpp $(CXXFLAGS) $($(COVERAGE)) -I include -I. -include test/test.hpp -o test/ft_except.out && $($(MEMCHECK)) test/ft_except.out

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

doc: doc_$(MKDOCS_THEME)

doc_%:
	cd doc && $(MKDOCS) build --clean --theme-dir themes/$* --site-dir $(MKDOCS_SITE)

doc_boost-classic:
	cd doc && python themes/boost-classic/scripts/update_markdown.py . https://raw.githubusercontent.com/boost-experimental/msm-lite/master
	cd doc && $(MKDOCS) build --clean --theme-dir themes/boost-classic --site-dir $(MKDOCS_SITE)

readme:
	cd doc && $(PYTHON) scripts/update_readme_toc.py mkdocs.yml ../README.md http://boost-experimental.github.io/msm-lite

clean:
	find example test -iname "*.out" | xargs rm -f

