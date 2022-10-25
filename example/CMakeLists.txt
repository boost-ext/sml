#
# Copyright (c) 2016-2020 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
project(example LANGUAGES CXX)

include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR})

source_group(example)

if(IS_COMPILER_GCC_LIKE)
  add_example(actions_guards actions_guards actions_guards.cpp)
  add_example(arduino arduino arduino.cpp)
  add_example(composite example_composite composite.cpp)
  add_example(data example_data data.cpp)
  add_example(defer_and_process example_defer_and_process defer_and_process.cpp)
  add_example(dependencies example_dependencies dependencies.cpp)
  add_example(dependency_injection example_dependency_injection
              dependency_injection.cpp)
  add_example(dispatch_table example_dispatch_table dispatch_table.cpp)
  add_example(dispatch_policy example_poilcy dispatch_policy.cpp)

  if(${SML_USE_EXCEPTIONS})
    add_example(error_handling example_error_handling error_handling.cpp)
    if(IS_COMPILER_OPTION_GCC_LIKE)
      target_compile_options(
        error_handling PRIVATE "-fexceptions") # Enable exception handling
    endif()
  endif()

  add_example(euml_emulation example_euml_emulation euml_emulation.cpp)
  add_example(events example_events events.cpp)
  if(IS_COMPILER_OPTION_GCC_LIKE)
    target_compile_options(events PRIVATE "-Wno-unused-parameter")
  endif()

  add_example(eval example_eval eval.cpp)
endif()

add_example(hello_world example_hello_world hello_world.cpp)

if(IS_COMPILER_GCC_LIKE)
  add_example(history example_history history.cpp)
  add_example(in_place example_in_place in_place.cpp)
  add_example(logging example_logging logging.cpp)
  add_example(nested example_nested nested.cpp)
  add_example(orthogonal_regions example_orthogonal_regions
              orthogonal_regions.cpp)
  add_example(plant_uml example_plant_uml plant_uml.cpp)
  add_example(sdl2 example_sdl2 sdl2.cpp)
  add_example(states example_states states.cpp)
  add_example(testing example_testing testing.cpp)
  add_example(transitions example_transitions transitions.cpp)
  add_example(visitor example_visitor visitor.cpp)
endif()
