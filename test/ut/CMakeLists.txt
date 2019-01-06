#
# Copyright (c) 2016-2019 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# include\boost/sml.hpp(1330): error C3779: 'c::operator ()': a function that returns 'auto' cannot be used before it is defined
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # gcc
    add_executable(test_concepts concepts.cpp)
    add_test(test_concepts test_concepts)
endif()

add_executable(test_type_traits type_traits.cpp)
add_test(test_type_traits test_type_traits)

add_executable(test_utility utility.cpp)
add_test(test_utility test_utility)
