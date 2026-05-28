#
# Copyright (c) 2016-2020 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Each target must be EXCLUDE_FROM_ALL so it is only built when ctest
# invokes cmake --build for that specific target (WILL_FAIL TRUE).
foreach(name not_callable not_configurable not_dispatchable not_transitional)
  add_library(error_${name} OBJECT EXCLUDE_FROM_ALL ${name}.cpp)
  target_link_libraries(error_${name} sml)
  add_test(
    NAME test_error_${name}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target error_${name}
  )
  set_tests_properties(test_error_${name} PROPERTIES WILL_FAIL TRUE)
endforeach()
