#
# Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
CXX?=clang++

.PHONY: all at unique

all: unique

at:
	cd . && time $(CXX) -std=c++14 at.cpp
	cd . && time $(CXX) -std=c++14 hana_at.cpp -I hana/include -stdlib=libc++
	cd . && time $(CXX) -std=c++11 mpl_at.cpp -ftemplate-depth=512

unique:
	cd . && time $(CXX) -std=c++14 unique.cpp
	cd . && time $(CXX) -std=c++14 hana_unique.cpp -I hana/include -stdlib=libc++ -ftemplate-depth=512
	cd . && time $(CXX) -std=c++11 mpl_unique.cpp -ftemplate-depth=1024

