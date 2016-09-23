#
# Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
.PHONY: all doc clean test example
CXX?=clang++
#CXXFLAGS:=-std=c++1y -Wall -Wextra -Werror -pedantic -pedantic-errors
CXXFLAGS:=-c -I "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\ClangC2\include" -I "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include" -I "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\atlmfc\include" -I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.10240.0\ucrt" -I "C:\Program Files (x86)\Windows Kits\8.1\Include\um" -I "C:\Program Files (x86)\Windows Kits\8.1\Include\shared" -I "C:\Program Files (x86)\Windows Kits\8.1\Include\winrt" -fdiagnostics-format=msvc -target "i686-pc-windows-msvc" -Wall -O3 -fno-strict-aliasing -fomit-frame-pointer -ffunction-sections -fdata-sections -fstack-protector -fpic -fno-short-enums -fno-rtti -std=c++1y -D WIN32 -D _WINDOWS -D _DEBUG -D "CMAKE_INTDIR=\"Debug\"" -D NDEBUG -D _MBCS -x c++ -fms-extensions -fno-ms-compatibility  -fms-extensions -fms-compatibility -frtti -fno-exceptions -pedantic -pedantic-errors -Wextra -Werror -gline-tables-only -fno-inline -O0 -fno-delayed-template-parsing  -D_DEBUG -D_MT -D_DLL -Xclang --dependent-lib=msvcrtd -Xclang --dependent-lib=oldnames
VALGRIND:=valgrind --leak-check=full --error-exitcode=1
GCOV:=-fprofile-arcs -ftest-coverage
CLANG_FORMAT?=clang-format
CLANG_TIDY?=clang-tidy
PYTHON?=python
MKDOCS?=mkdocs
MKDOCS_THEME?=boost-experimental
MKDOCS_SITE?=site

all: test example

test: $(patsubst %.cpp, %.out, $(wildcard test/*.cpp))

test/%.out:
	$(CXX) test/$*.cpp $(CXXFLAGS) -fno-exceptions $($(COVERAGE)) -I include -I. -include test/test.hpp -o test/$*.out 
	link.exe /OUT: test/$*.exe /NOLOGO kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib
	$($(MEMCHECK)) test/$*.exe
#	&& $($(MEMCHECK)) test/$*.out

test/ft_except.out:
	$(CXX) test/ft_except.cpp $(CXXFLAGS) $($(COVERAGE)) -I include -I. -include test/test.hpp -o test/ft_except.out && $($(MEMCHECK)) test/ft_except.out

test/ft_thread.out: #-fsanitize=thread
	$(CXX) test/ft_thread.cpp $(CXXFLAGS) -fno-exceptions -DBOOST_MSM_LITE_THREAD_SAFE -lpthread $($(COVERAGE)) -I include -I. -include test/test.hpp -o test/ft_thread.out && $($(MEMCHECK)) test/ft_thread.out

test/ft_unit1.out:
	$(CXX) test/ft_unit1.cpp $(CXXFLAGS) $($(COVERAGE)) -I include -c -o test/ft_unit1.out

test/ft_unit2.out:
	$(CXX) test/ft_unit2.cpp $(CXXFLAGS) $($(COVERAGE)) -I include -c -o test/ft_unit2.out

test/ft_units.out: test/ft_unit1.out test/ft_unit2.out
	$(CXX) test/ft_units.cpp $(CXXFLAGS) -fno-exceptions $($(COVERAGE)) -I include -I. -include test/test.hpp test/ft_unit1.out test/ft_unit2.out -o test/ft_units.out

example: $(patsubst %.cpp, %.out, $(wildcard example/*.cpp))

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

