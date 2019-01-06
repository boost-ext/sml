#
# Copyright (c) 2016-2019 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
add_executable(simple_euml euml.cpp)
add_test(benchmark_simple_euml simple_euml)

# sml/benchmark/complex/euml2.cpp:14:10: fatal error: boost/msm/front/euml2/euml2.hpp: No such file or directory
# add_executable(simple_euml2 euml2.cpp)

add_executable(simple_sc sc.cpp)
add_test(benchmark_simple_sc simple_sc)

if (NOT IS_MSVC_2015)
	add_executable(simple_sml sml.cpp)
	add_test(benchmark_simple_sml simple_sml)
endif()
