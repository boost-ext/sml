# Generate stage:
#   $mkdirBUILD; cd BUILD; cmake ..
#   Re-generate after changes:
#     $cmake .

# Build stage:
#   Linux: $cmake --build . -- -j
#   Windows: $cmake --build . -- /maxcpucount
#   Build only test/action_defer:
#   $cmake --build . --target actions_defer

# Test stage:
#    $ctest --verbose

# TODO
# - translate sml/benchmark/Makefile
# - translate sml/doc/cppnow-2016/example/Makefile
# - merge 02cf41c691 [appveyor] Fix for MSVC
# - In test/ft/errors/CMakeLists.txt
#     Implement ctest_build to check build failure
#     https://cmake.org/cmake/help/latest/manual/ctest.1.html#ctest-build-step
#     https://cmake.org/cmake/help/latest/command/ctest_build.html#command:ctest_build

cmake_minimum_required(VERSION 3.4)
project(sml LANGUAGES CXX)
include(CTest)

# show each build command line as it is launched
set(CMAKE_VERBOSE_MAKEFILE "ON")

message("CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 19.12 OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 19.12)
    set(CMAKE_CXX_STANDARD 17)
  endif()
else()
  set(CMAKE_CXX_STANDARD 14)
endif()

set(CXX_STANDARD_REQUIRED ON)

# Turn on the .vcproj folder support
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Boost          REQUIRED)

include_directories(AFTER
    ${Boost_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(
      -DNOMINMAX # avoid Win macro definition of min/max, use std one
      -D_SCL_SECURE_NO_WARNINGS # disable security-paranoia warning
      -D_CRT_SECURE_NO_WARNINGS)
    add_compile_options(
      "/W3" # waring level
      "/WX" # warning as error
      )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # gcc
    # https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
    add_compile_options(
      "-Wfatal-errors" # stops on first error
      "-Wall" # enables all the warnings about constructions
      "-Wextra" # Print extra warning messages"
      "-Werror" # Make all warnings into errors.
      "-pedantic" # Issue all the warnings demanded by strict ISO C and ISO C++
      "-pedantic-errors" # Like -pedantic, except that errors are produced rather than warnings
      "-fno-exceptions" # compiles without exception support
      )

    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.7)
        # http://stackoverflow.com/questions/30255294/how-to-hide-extra-output-from-pragma-message
        add_compile_options("-ftrack-macro-expansion=0;-fno-diagnostics-show-caret")
    endif ()
endif ()

#add_subdirectory(benchmark)
set(DOC_AS_TEST ON)
add_subdirectory(doc)
#add_subdirectory(example)
#add_subdirectory(test)
