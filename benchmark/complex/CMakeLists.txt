#
# Copyright (c) 2016-2020 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
if (IS_COMPILER_GCC_LIKE)
  set(CURRENT_CXX_STANDARD ${CMAKE_CXX_STANDARD})
  set(CMAKE_CXX_STANDARD 11)
  add_example(complex_euml benchmark_complex_euml euml.cpp)
  set(CMAKE_CXX_STANDARD ${CURRENT_CXX_STANDARD})
endif()

add_executable(complex_euml2 euml2.cpp)
add_example(complex_sc benchmark_complex_sc sc.cpp)

if (NOT IS_MSVC_2015)
  add_example(complex_sml benchmark_complex_sml sml.cpp)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(complex_sml PRIVATE
        "-Wno-subobject-linkage"
        "-Wno-fatal-errors")
  endif()
endif()

add_executable(switch switch.cpp)
add_test(benchmark_switch switch)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(switch PRIVATE
        "-Wno-maybe-uninitialized")
endif()

if (${CMAKE_CXX_STANDARD} GREATER_EQUAL 17)
  #add_executable(variant variant.cpp)
endif()
