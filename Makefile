#
# Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
.PHONY: all doc clean test example pt
CXX?=clang++
CXXFLAGS:=-std=c++1y -Wall -Wextra -Werror -pedantic -pedantic-errors -fno-exceptions
VALGRIND:=valgrind --leak-check=full --error-exitcode=1
CLANG_FORMAT?=clang-format
CLANG_TIDY?=clang-tidy
PYTHON?=python
MKDOCS?=mkdocs

define UPDATE_README
import fileinput, sys
generating = False
for line in fileinput.input('../README.md', inplace=True):
  if line.startswith('[](GENERATE_TOC_BEGIN)'):
    generating = True
    print(line)
    with open('mkdocs.yml', 'r') as file:
      for line in file:
        index = line.split(':')
        if index[0][0] == '-':
          ext = ('.html' if index[1][1:-4] == 'index' else '/index.html')
          print('* [' + index[0][2:] + '](http://boost-experimental.github.io/msm-lite/' + index[1][1:-4] + ext + ')')
          with open(index[1][1:-1], 'r') as md:
            for line in md:
              if line.startswith('##'):
                name = line.replace('#', '')[:-1]
                id = filter(lambda c: c == '-' or str.isalnum(c), name.lower().replace(" ", "-")).replace("--", "")
                print('    * [' + name + '](http://boost-experimental.github.io/msm-lite/' + index[1][1:-4] + ext + "#" + id + ')')
    print
  elif line.startswith('[](GENERATE_TOC_END)'):
    generating = False
    sys.stdout.write(line)
  elif generating == False:
    sys.stdout.write(line)
endef
export UPDATE_README

all: test example pt

pt: pt_header pt_simple pt_composite pt_complex

pt_%:
	time $(CXX) test/pt/$*/msm-lite.cpp -O2 -s -I include -I test/pt -std=c++1y && ./a.out && ls -lh a.out
	time $(CXX) test/pt/$*/sc.cpp -ftemplate-depth=1024 -O2 -s -I include -I test/pt -std=c++1y && ./a.out && ls -lh a.out
	time $(CXX) test/pt/$*/euml.cpp -ftemplate-depth=1024 -O2 -s -I include -I test/pt -std=c++1y && ./a.out && ls -lh a.out

test: $(patsubst %.cpp, %.out, $(shell find test -maxdepth 1 -iname "*.cpp"))

test/%.out:
	$(CXX) test/$*.cpp $(CXXFLAGS) -I include -I. -include test/test.hpp -o test/$*.out && $($(MEMCHECK)) test/$*.out

example: $(patsubst %.cpp, %.out, $(shell find example -iname "*.cpp"))

example/%.out:
	$(CXX) example/$*.cpp $(CXXFLAGS) -I include -o example/$*.out && $($(MEMCHECK)) example/$*.out

example/errors/%.out:
	-$(CXX) example/errors/$*.cpp $(CXXFLAGS) -I include

check: check_style check_static_analysis

check_style:
	@find include example test -iname "*.hpp" -or -iname "*.cpp" | xargs $(CLANG_FORMAT) -i
	@git diff include example test
	@exit `git ls-files -m include example test | wc -l`

check_static_analysis:
	$(CLANG_TIDY) -header-filter='msm' `find example test -type f -iname "*.cpp"` -- -std=c++1y -I include -I test -include test.hpp

doc:
	cd doc && $(MKDOCS) build --clean && $(PYTHON) -c "$$UPDATE_README"

clean:
	find example test -iname "*.out" | xargs rm -f

