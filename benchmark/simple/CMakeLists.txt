#
# Copyright (c) 2016-2020 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
set(CURRENT_CXX_STANDARD ${CMAKE_CXX_STANDARD})
set(CMAKE_CXX_STANDARD 11)
add_example(simple_euml benchmark_simple_euml euml.cpp)
set(CMAKE_CXX_STANDARD ${CURRENT_CXX_STANDARD})

add_example(simple_sc benchmark_simple_sc sc.cpp)

if (NOT IS_MSVC_2015)
  add_example(simple_sml benchmark_simple_sml sml.cpp)
endif()
