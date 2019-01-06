#
# Copyright (c) 2016-2019 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
project(example LANGUAGES CXX)

include_directories(AFTER
    ${CMAKE_CURRENT_SOURCE_DIR})

source_group(example)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_executable(actions_guards actions_guards.cpp)
    add_test(example_actions_guards actions_guards)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(actions_guards PRIVATE
            "-Wno-unused-parameter")
    endif()

    add_executable(composite composite.cpp)
    add_test(example_composite composite)

    add_executable(data data.cpp)
    add_test(example_data data)

    add_executable(defer_and_process defer_and_process.cpp)
    add_test(example_defer_and_process defer_and_process)

    add_executable(dependencies dependencies.cpp)
    add_test(example_dependencies dependencies)
endif()

if (NOT IS_MSVC_2015)
    add_executable(dependency_injection dependency_injection.cpp)
    add_test(example_dependency_injection dependency_injection)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_executable(dispatch_table dispatch_table.cpp)
    add_test(example_dispatch_table dispatch_table)

    add_executable(error_handling error_handling.cpp)
    add_test(example_error_handling error_handling)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(error_handling PRIVATE
            "-fexceptions") # Enable exception handling
    endif()
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_executable(euml_emulation euml_emulation.cpp)
    add_test(example_euml_emulation euml_emulation)

    add_executable(events events.cpp)
    add_test(example_events events)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(events PRIVATE
            "-Wno-unused-parameter")
    endif()
endif()

if (NOT IS_MSVC_2015)
    add_executable(hello_world hello_world.cpp)
    add_test(example_hello_world hello_world)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_executable(history history.cpp)
    add_test(example_history history)

    add_executable(logging logging.cpp)
    add_test(example_logging logging)

    add_executable(nested nested.cpp)
    add_test(example_nested nested)

    add_executable(orthogonal_regions orthogonal_regions.cpp)
    add_test(example_orthogonal_regions orthogonal_regions)

    add_executable(plant_uml plant_uml.cpp)
    add_test(example_plant_uml plant_uml)

    add_executable(sdl2 sdl2.cpp)
    add_test(example_sdl2 sdl2)

    add_executable(states states.cpp)
    add_test(example_states states)

    add_executable(testing testing.cpp)
    add_test(example_testing testing)

    add_executable(transitions transitions.cpp)
    add_test(example_transitions transitions)
endif()
