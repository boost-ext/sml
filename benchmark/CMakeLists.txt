#
# Copyright (c) 2016-2020 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
project(benchmark LANGUAGES CXX)

include_directories(AFTER
  ${CMAKE_CURRENT_SOURCE_DIR}
  header)

add_definitions(
  -DCHECK_COMPILE_TIME
  -DBOOST_ALL_NO_LIB
  -DBUILD_SHARED_LIBS=OFF)

if (IS_COMPILER_GCC_LIKE)
  add_compile_options(
    "-Wno-unused-local-typedefs"
    "-Wno-unused-parameter")
endif ()

add_subdirectory(complex)
add_subdirectory(composite)
add_subdirectory(header)
add_subdirectory(simple)
