#
# Copyright (c) 2016-2020 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
set(CURRENT_CXX_STANDARD ${CMAKE_CXX_STANDARD})
set(CMAKE_CXX_STANDARD 11)
add_example(composite_euml benchmark_composite_euml euml.cpp)
set(CMAKE_CXX_STANDARD ${CURRENT_CXX_STANDARD})

add_example(composite_sc benchmark_composite_sc sc.cpp)

if (NOT IS_MSVC_2015)
  add_example(composite_sml benchmark_composite_sml sml.cpp)

  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(composite_sml PRIVATE
      "-Wno-subobject-linkage"
      "-Wno-fatal-errors")
  endif()
endif()

