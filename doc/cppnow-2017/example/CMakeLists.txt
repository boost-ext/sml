project(doc LANGUAGES CXX)

add_subdirectory(cppnow-2016)
add_subdirectory(cppnow-2017)


add_executable(god_object
    doc/cppnow-2017/example/god_object.cpp)

add_executable(at
  doc/cppnow-2016/example/at.cpp)
add_executable(hana_at
  doc/cppnow-2016/example/hana_at.cpp)
add_executable(hana_unique
  doc/cppnow-2016/example/hana_unique.cpp)
add_executable(mpl_at
  doc/cppnow-2016/example/mpl_at.cpp)
add_executable(mpl_unique
  doc/cppnow-2016/example/mpl_unique.cpp)
add_executable(unique
  doc/cppnow-2016/example/unique.cpp)
add_executable(god_object
  doc/cppnow-2017/example/god_object.cpp)
add_executable(static_polymorphism
  doc/cppnow-2017/example/static_polymorphism.cpp)
add_executable(type_erasure
  doc/cppnow-2017/example/type_erasure.cpp)
add_executable(connection_v1
  doc/embo-2017/example/connection_v1.cpp)
# doc/embo-2017/example/connection_v2.cpp
# doc/embo-2017/example/connection_v3.cpp
# doc/embo-2017/example/connection_v4.cpp
# example/actions_guards.cpp
# example/composite.cpp
# example/defer_and_process.cpp
# example/dependencies.cpp
# example/dependency_injection.cpp
# example/dispatch_table.cpp
# example/error_handling.cpp
# example/euml_emulation.cpp
# example/events.cpp
# example/hello_world.cpp
# example/history.cpp
# example/logging.cpp
# example/orthogonal_regions.cpp
# example/plant_uml.cpp
# example/sdl2.cpp
# example/states.cpp
# example/testing.cpp
# example/transitions.cpp
# test/ft/actions_defer.cpp
# test/ft/actions_process.cpp
# test/ft/composite.cpp
# test/ft/dependencies.cpp
# test/ft/di.cpp
# test/ft/dispatch_table.cpp
# test/ft/events.cpp
# test/ft/exceptions.cpp
# test/ft/fwd.cpp
# test/ft/history.cpp
# test/ft/orthogonal_regions.cpp
# test/ft/policies_logging.cpp
# test/ft/policies_testing.cpp
# test/ft/policies_thread_safe.cpp
# test/ft/sizeof.cpp
# test/ft/states.cpp
# test/ft/state_machine.cpp
# test/ft/transitions.cpp
# test/ft/transition_table.cpp
# test/ft/unexpected_events.cpp
# test/ft/unit1.cpp
# test/ft/unit2.cpp
# test/ft/units.cpp
# test/ft/errors/not_callable.cpp
# test/ft/errors/not_configurable.cpp
# test/ft/errors/not_dispatchable.cpp
# test/ft/errors/not_transitional.cpp
# test/ut/concepts.cpp
# test/ut/type_traits.cpp
# test/ut/utility.cpp
