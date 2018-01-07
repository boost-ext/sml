add_executable(connection_v1 connection_v1.cpp)
add_test(doc_embo_2017_connection_v1 connection_v1)

# doc\embo-2017\example\connection_v4.cpp(21): error C3688: invalid literal suffix '_s'; literal operator or literal operator template 'operator ""_s' not found
# Visual Studio 15 2017 failure
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_executable(connection_v2 connection_v2.cpp)
  add_test(doc_embo_2017_connection_v2 connection_v2)
  target_compile_options(connection_v2 PRIVATE
      "-Wno-uninitialized")

  add_executable(connection_v4 connection_v4.cpp)
  add_test(doc_embo_2017_connection_v4 connection_v4)
endif()

if(${CMAKE_CXX_STANDARD} GREATER_EQUAL 17)
  #add_executable(connection_v3 connection_v3.cpp)
  #add_test(doc_embo_2017_connection_v3 connection_v3)

# In file included from /mnt/c/dev/sml/doc/embo-2017/example/connection_v3.cpp:2:0:
# /usr/include/c++/7/variant: In instantiation of ‘constexpr decltype(auto) std::visit(_Visitor&&, _Variants&& ...)
#     [with _Visitor = overload<>; _Variants = {std::variant<ConnectionV3::Disconnected, ConnectionV3::Connecting, ConnectionV3::Connected>&}]’:
# /mnt/c/dev/sml/doc/embo-2017/example/connection_v3.cpp:32:17:   required from here
# /usr/include/c++/7/variant:1242:44: error: no match for call to ‘(overload<>)
#     (std::variant_alternative_t<0, std::variant<ConnectionV3::Disconnected, ConnectionV3::Connecting, ConnectionV3::Connected> >&)’
# compilation terminated due to -Wfatal-errors.

endif()
