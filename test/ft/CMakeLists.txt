add_executable(test_actions_defer actions_defer.cpp)
add_test(
    NAME test_actions_defer
    COMMAND test_actions_defer
    # execute_process()
    #-DTEST_PROG=$<TARGET_FILE:test_actions_defer>
    #-DSOURCEDIR=${CMAKE_CURRENT_SOURCE_DIR}
    )
#add_test(NAME runtestprog
#    COMMAND ${CMAKE_COMMAND}
#    -DTEST_PROG=$<TARGET_FILE:testprog>
#    -DSOURCEDIR=${CMAKE_CURRENT_SOURCE_DIR}
#    -P ${CMAKE_CURRENT_SOURCE_DIR}/runtest.cmake)

add_executable(test_actions_process actions_process.cpp)
add_test(
    NAME test_actions_process
    COMMAND ./test_actions_process
    # execute_process()
    -DTEST_PROG=$<TARGET_FILE:test_actions_process>
    -DSOURCEDIR=${CMAKE_CURRENT_SOURCE_DIR}
    )

add_executable(test_composite composite.cpp)

add_executable(test_dependencies dependencies.cpp)

add_executable(test_di di.cpp)

add_executable(test_dispatch_table dispatch_table.cpp)

add_executable(test_events events.cpp)

add_executable(test_exceptions exceptions.cpp)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # gcc
    target_compile_options(test_exceptions PRIVATE
        "-fexceptions" # Enable exception handling
        )
endif ()

add_executable(test_fwd fwd.cpp)

add_executable(test_history history.cpp)

add_executable(test_orthogonal_regions orthogonal_regions.cpp)

add_executable(test_policies_logging policies_logging.cpp)

add_executable(test_policies_testing policies_testing.cpp)

add_executable(test_policies_thread_safe policies_thread_safe.cpp)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # gcc
    target_link_libraries(test_policies_thread_safe
        -lpthread)
endif ()

add_executable(test_sizeof sizeof.cpp)

add_executable(test_state_machine state_machine.cpp)

add_executable(test_states states.cpp)

add_executable(test_transitions transitions.cpp)

add_executable(test_transition_table transition_table.cpp)

add_executable(test_unexpected_events unexpected_events.cpp)

add_executable(test_unit unit1.cpp unit2.cpp unit2.cpp units.cpp)

add_subdirectory(errors)
