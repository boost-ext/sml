#
# Copyright (c) 2016-2020 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Generate stage:
#   $mkdir build; cd build; cmake ..
#   Re-generate after changes:
#     $cmake .
#   Show each build command line as it is launched
#   $cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..
#   Generate a release version: $cmake -DCMAKE_BUILD_TYPE=Release ..

# Build stage:
#   Linux: $cmake --build . -- -j
#   Windows: $cmake --build . -- /maxcpucount
#   Build only test/action_defer:
#   $cmake --build . --target actions_defer

# Test stage:
#    Launch all tests: $ctest --verbose
#    To run tests with memcheck: $ctest -T memcheck
#    List available test: $ctest -N
#    Launch benchmark tests: $ctest -R benchmark*

cmake_minimum_required(VERSION 3.10)
project(sml LANGUAGES CXX)
include(CTest)

option(SML_BUILD_BENCHMARKS "Build benchmarks" ON)
option(SML_BUILD_DOCS       "Build docs"       ON)
option(SML_BUILD_EXAMPLES   "Build examples"   ON)
option(SML_BUILD_TESTS      "Build tests"      ON)
option(SML_USE_EXCEPTIONS   "Do not disable exceptions" ON)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 6.0.0)
  message(FATAL_ERROR "sml requires GCC >= 6.0.0")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3.5.0)
  message(FATAL_ERROR "sml requires Clang >= 3.5.0")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.0)
  message(FATAL_ERROR "sml requires Visual Studio 14 2015 at least")
endif()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=c++14 HAS_CXX14_FLAG)
check_cxx_compiler_flag(-std=c++17 HAS_CXX17_FLAG)
check_cxx_compiler_flag(-std=c++2a HAS_CXX20_FLAG)

if(HAS_CXX20_FLAG)
  set(CMAKE_CXX_STANDARD 20)
elseif(HAS_CXX17_FLAG)
  set(CMAKE_CXX_STANDARD 17)
elseif(HAS_CXX14_FLAG)
  set(CMAKE_CXX_STANDARD 14)
else()
  message(FATAL_ERROR "sml requires c++14")
endif()

if(NOT (DEFINED CMAKE_CXX_STANDARD) OR CMAKE_CXX_STANDARD STREQUAL "" OR CMAKE_CXX_STANDARD LESS 14)
    message(FATAL_ERROR "sml requires c++14")
endif()

set(IS_COMPILER_GCC_LIKE FALSE)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )

  set(IS_COMPILER_GCC_LIKE TRUE)
endif()

include(GNUInstallDirs)

install(DIRECTORY "include/boost" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

set(CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Boost REQUIRED)

add_library(sml INTERFACE)

target_include_directories(sml
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(sml
  INTERFACE Boost::boost)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  target_compile_definitions(sml
    INTERFACE NOMINMAX # avoid Win macro definition of min/max, use std one
    INTERFACE _SCL_SECURE_NO_WARNINGS # disable security-paranoia warning
    INTERFACE _CRT_SECURE_NO_WARNINGS)
  if(CMAKE_CXX_STANDARD GREATER_EQUAL 17)
    target_compile_definitions(sml
      INTERFACE _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
  endif()
elseif (IS_COMPILER_GCC_LIKE) # gcc
  # https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
  target_compile_options(sml
    INTERFACE "-Wfatal-errors" # stops on first error
    INTERFACE "-Wall" # enables all the warnings about constructions
    INTERFACE "-Wextra" # Print extra warning messages"
    INTERFACE "-Werror" # Make all warnings into errors.
    INTERFACE "-pedantic" # Issue all the warnings demanded by strict ISO C and ISO C++
    INTERFACE "-pedantic-errors" # Like -pedantic, except that errors are produced rather than warnings
  )

  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(sml
      # http://stackoverflow.com/questions/30255294/how-to-hide-extra-output-from-pragma-message
      INTERFACE "-ftrack-macro-expansion=0"
      INTERFACE "-fno-diagnostics-show-caret"
    )
  endif()

  if (NOT ${SML_USE_EXCEPTIONS})
    target_compile_options(sml
      INTERFACE "-fno-exceptions" # compiles without exception support
      )
  endif()
endif ()

include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

function(add_example exe_name bm_name src_name)
  add_executable(${exe_name} ${src_name})
  target_link_libraries(${exe_name} PUBLIC sml)
  add_test(${bm_name} ${exe_name})
endfunction()

if (SML_BUILD_BENCHMARKS)
  add_subdirectory(benchmark)
endif()

if (SML_BUILD_DOCS)
  add_subdirectory(doc)
endif()

if (SML_BUILD_EXAMPLES)
  add_subdirectory(example)
endif()

if (SML_BUILD_TESTS)
  add_subdirectory(test)
endif()
