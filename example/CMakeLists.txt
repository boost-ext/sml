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

if (IS_COMPILER_GCC_LIKE)
    add_executable(actions_guards actions_guards.cpp)
    target_link_libraries(actions_guards PUBLIC sml) 
    add_test(example_actions_guards actions_guards)

    if (IS_COMPILER_GCC_LIKE)
        target_compile_options(actions_guards PRIVATE
            "-Wno-unused-parameter")
    endif()

    add_example(composite example_composite composite.cpp)

    add_example(data example_data data.cpp)

    add_example(defer_and_process example_defer_and_process defer_and_process.cpp)

    add_example(dependencies example_dependencies dependencies.cpp)
endif()

if (NOT IS_MSVC_2015)
    add_executable(dependency_injection dependency_injection.cpp)
    add_test(example_dependency_injection dependency_injection)
endif()

if (IS_COMPILER_GCC_LIKE)
    add_example(dispatch_table example_dispatch_table dispatch_table.cpp)

    if (${SML_USE_EXCEPTIONS})
    add_executable(error_handling error_handling.cpp)
    target_link_libraries(error_handling PUBLIC sml)
    add_test(example_error_handling error_handling)
    if (IS_COMPILER_GCC_LIKE)
        target_compile_options(error_handling PRIVATE
        "-fexceptions") # Enable exception handling
    endif()
    endif()
endif()

if (IS_COMPILER_GCC_LIKE)
    add_example(euml_emulation example_euml_emulation euml_emulation.cpp)

    add_executable(events events.cpp)
    target_link_libraries(events PUBLIC sml) 
    add_test(example_events events)

    if (IS_COMPILER_GCC_LIKE)
        target_compile_options(events PRIVATE
            "-Wno-unused-parameter")
    endif()
endif()

if (NOT IS_MSVC_2015)
    add_example(hello_world example_hello_world hello_world.cpp)
endif()

if (IS_COMPILER_GCC_LIKE)
    add_example(history example_history history.cpp)

    add_example(logging example_logging logging.cpp)

    add_example(nested example_nested nested.cpp)

    add_example(orthogonal_regions example_orthogonal_regions orthogonal_regions.cpp)

    add_example(plant_uml example_plant_uml plant_uml.cpp)

    add_example(sdl2 example_sdl2 sdl2.cpp)

    add_example(states example_states states.cpp)

    add_example(testing example_testing testing.cpp)

    add_example(transitions example_transitions transitions.cpp)
endif()
