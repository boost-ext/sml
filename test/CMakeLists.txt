include_directories(AFTER
    common)

include_directories(AFTER
    ${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(
    -DCHECK_COMPILE_TIME
    -DBOOST_ALL_NO_LIB
    -DBUILD_SHARED_LIBS=OFF)

project(test LANGUAGES CXX)

add_subdirectory(ft)
add_subdirectory(ut)

