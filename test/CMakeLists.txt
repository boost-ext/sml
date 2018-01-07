project(test LANGUAGES CXX)

include_directories(AFTER
    ${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(
    -DCHECK_COMPILE_TIME
    -DBOOST_ALL_NO_LIB
    -DBUILD_SHARED_LIBS=OFF)

add_subdirectory(unit)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options("-FI common/test.hpp")
else()
    add_compile_options(-include common/test.hpp)
endif()

add_subdirectory(ft)
add_subdirectory(ut)

