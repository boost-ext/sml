
add_executable(actions_defer actions_defer.cpp)

add_executable(actions_process actions_process.cpp)

add_executable(composite composite.cpp)

add_executable(dependencies dependencies.cpp)

add_executable(di di.cpp)

add_executable(dispatch_table dispatch_table.cpp)

add_executable(events events.cpp)

add_executable(exceptions exceptions.cpp)

add_executable(fwd fwd.cpp)

add_executable(history history.cpp)

add_executable(orthogonal_regions orthogonal_regions.cpp)

add_executable(policies_logging policies_logging.cpp)

add_executable(policies_testing policies_testing.cpp)

add_executable(policies_thread_safe policies_thread_safe.cpp)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # gcc
    target_link_libraries(policies_thread_safe
        -lpthread)
endif ()

add_executable(sizeof sizeof.cpp)

add_executable(state_machine state_machine.cpp)

add_executable(states states.cpp)

add_executable(transitions transitions.cpp)

add_executable(transition_table transition_table.cpp)

add_executable(unexpected_events unexpected_events.cpp)

add_executable(unit unit1.cpp unit2.cpp unit2.cpp units.cpp)

add_subdirectory(errors)