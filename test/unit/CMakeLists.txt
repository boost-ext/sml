#
# Copyright (c) 2016-2020 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
add_executable(test_unit unit1.cpp unit2.cpp units.cpp)
target_link_libraries(test_unit PUBLIC sml)
add_test(test_unit test_unit)
