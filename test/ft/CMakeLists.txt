#
# Copyright (c) 2016-2019 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# include\boost/sml.hpp(1330): error C3779: 'c::operator ()': a function that returns 'auto' cannot be used before it is defined
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # gcc
    add_executable(test_actions_defer actions_defer.cpp)
    add_test(test_actions_defer test_actions_defer)

    add_executable(test_actions_process actions_process.cpp)
    add_test(test_actions_process test_actions_process)

    add_executable(test_composite composite.cpp)
    add_test(test_composite test_composite)

    add_executable(test_dependencies dependencies.cpp)
    add_test(test_dependencies test_dependencies)
endif()

add_executable(test_di di.cpp)
add_test(test_di test_di)

# include\boost/sml.hpp(1330): error C3779: 'c::operator ()': a function that returns 'auto' cannot be used before it is defined
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # gcc
    add_executable(test_dispatch_table dispatch_table.cpp)
    add_test(test_dispatch_table test_dispatch_table)
endif ()

add_executable(test_events events.cpp)
add_test(test_events test_events)

add_executable(test_exceptions exceptions.cpp)
add_test(test_exceptions test_exceptions)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # gcc
    target_compile_options(test_exceptions PRIVATE
        "-fexceptions") # Enable exception handling

    add_executable(test_fwd fwd.cpp)
    add_test(test_fwd test_fwd)
endif ()

# include\boost/sml.hpp(1330): error C3779: 'c::operator ()': a function that returns 'auto' cannot be used before it is defined
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # gcc
    add_executable(test_history history.cpp)
    add_test(test_history test_history)

    add_executable(test_orthogonal_regions orthogonal_regions.cpp)
    add_test(test_orthogonal_regions test_orthogonal_regions)
endif ()

add_executable(test_policies_logging policies_logging.cpp)
add_test(test_policies_logging test_policies_logging)

# include\boost/sml.hpp(1330): error C3779: 'c::operator ()': a function that returns 'auto' cannot be used before it is defined
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # gcc
    add_executable(test_policies_testing policies_testing.cpp)
    add_test(test_policies_testing test_policies_testing)

    add_executable(test_policies_dispatch policies_dispatch.cpp)
    add_test(test_policies_dispatch test_policies_dispatch)

    add_executable(test_policies_thread_safe policies_thread_safe.cpp)
    add_test(test_policies_thread_safe test_policies_thread_safe)
    target_link_libraries(test_policies_thread_safe
        -lpthread)
endif ()

add_executable(test_sizeof sizeof.cpp)
add_test(test_sizeof test_sizeof)

add_executable(test_constexpr constexpr.cpp)
add_test(test_constexpr test_constexpr)

# include\boost/sml.hpp(1330): error C3779: 'c::operator ()': a function that returns 'auto' cannot be used before it is defined
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # gcc
    add_executable(test_state_machine state_machine.cpp)
    add_test(test_state_machine test_state_machine)

    add_executable(test_states states.cpp)
    add_test(test_states test_states)

    add_executable(test_transitions transitions.cpp)
    add_test(test_transitions test_transitions)

    add_executable(test_transition_table transition_table.cpp)
    add_test(test_transition_table test_transition_table)

    add_executable(test_unexpected_events unexpected_events.cpp)
    add_test(test_unexpected_events test_unexpected_events)
endif ()

add_subdirectory(errors)
