project(example LANGUAGES CXX)

include_directories(AFTER
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIR})

source_group(example)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_executable(actions_guards actions_guards.cpp)
    add_test(actions_guards actions_guards)

    add_executable(composite composite.cpp)
    add_test(composite composite)

    add_executable(defer_and_process defer_and_process.cpp)
    add_test(defer_and_process defer_and_process)

    add_executable(dependencies dependencies.cpp)
    add_test(dependencies dependencies)
endif()

add_executable(dependency_injection dependency_injection.cpp)
add_test(dependency_injection dependency_injection)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_executable(dispatch_table dispatch_table.cpp)
    add_test(dispatch_table dispatch_table)

    add_executable(error_handling error_handling.cpp)
    add_test(error_handling error_handling)
    target_compile_options(error_handling PRIVATE
        "-fexceptions" # Enable exception handling
      )
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_executable(euml_emulation euml_emulation.cpp)
    add_test(euml_emulation euml_emulation)

    add_executable(events events.cpp)
    add_test(events events)
endif()

add_executable(hello_world hello_world.cpp)
add_test(hello_world hello_world)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_executable(history history.cpp)
    add_test(history history)

    add_executable(logging logging.cpp)
    add_test(logging logging)

    add_executable(orthogonal_regions orthogonal_regions.cpp)
    add_test(orthogonal_regions orthogonal_regions)

    add_executable(plant_uml plant_uml.cpp)
    add_test(plant_uml plant_uml)

    add_executable(sdl2 sdl2.cpp)
    add_test(sdl2 sdl2)

    add_executable(states states.cpp)
    add_test(states states)

    add_executable(testing testing.cpp)
    add_test(testing testing)

    add_executable(transitions transitions.cpp)
    add_test(transitions transitions)
endif()
