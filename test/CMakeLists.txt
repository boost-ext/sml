#
# Copyright (c) 2016-2020 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
project(test LANGUAGES CXX)

include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DCHECK_COMPILE_TIME -DBOOST_ALL_NO_LIB -DBUILD_SHARED_LIBS=OFF)

add_subdirectory(unit)

if(IS_COMPILER_OPTION_MSVC_LIKE)
  add_compile_options("-FIcommon/test.hpp")
elseif(IS_COMPILER_OPTION_GCC_LIKE)
  add_compile_options(-include common/test.hpp)
endif()

add_subdirectory(ft)
add_subdirectory(ut)
