project(example LANGUAGES CXX)

include_directories(AFTER
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIR})

source_group(example)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_executable(actions_guards
        actions_guards.cpp)
    add_executable(composite
        composite.cpp)
    add_executable(defer_and_process
        defer_and_process.cpp)
    add_executable(dependencies
        dependencies.cpp)
endif()

add_executable(dependency_injection
    dependency_injection.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_executable(dispatch_table
        dispatch_table.cpp)

    add_executable(error_handling
        error_handling.cpp)
    target_compile_options(error_handling PRIVATE
        "-fexceptions" # Enable exception handling
      )
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_executable(euml_emulation
        euml_emulation.cpp)
    add_executable(events
        events.cpp)
endif()

add_executable(hello_world
    hello_world.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_executable(history
        history.cpp)
    add_executable(logging
        logging.cpp)
    add_executable(orthogonal_regions
        orthogonal_regions.cpp)
    add_executable(plant_uml
        plant_uml.cpp)
    add_executable(sdl2
        sdl2.cpp)
    add_executable(states
        states.cpp)
    add_executable(testing
        testing.cpp)
    add_executable(transitions
        transitions.cpp)
endif()
