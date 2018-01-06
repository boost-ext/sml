add_executable(god_object god_object.cpp)

add_executable(static_polymorphism static_polymorphism.cpp)

#/mnt/c/dev/sml/doc/cppnow-2017/example/type_erasure.cpp: In constructor ‘FileReader::FileReader(const string&)’:
#/mnt/c/dev/sml/doc/cppnow-2017/example/type_erasure.cpp:19:54: error: ‘path’ was not declared in this scope
message( WARNING "doc/cppnow-2017/example/type_erasure.cpp build is disabled because \‘path\’ was not declared in this scope")
#add_executable(type_erasure type_erasure.cpp)

#In file included from /mnt/c/dev/boost_1_65_1/boost/type_erasure/builtin.hpp:17:0,
#                 from /mnt/c/dev/boost_1_65_1/boost/type_erasure/detail/normalize.hpp:34,
#                 from /mnt/c/dev/boost_1_65_1/boost/type_erasure/any.hpp:36,
#                 from /mnt/c/dev/sml/doc/cppnow-2017/example/type_erasure.cpp:1:
#/mnt/c/dev/boost_1_65_1/boost/type_erasure/constructible.hpp:44:2: error: extra ‘;’ [-Wpedantic]

#In file included from /mnt/c/dev/boost_1_65_1/boost/preprocessor/iteration/detail/iter/forward1.hpp:47:0,
#                 from /mnt/c/dev/boost_1_65_1/boost/type_erasure/detail/instantiate.hpp:57,
#                 from /mnt/c/dev/boost_1_65_1/boost/type_erasure/any.hpp:38,
#                 from /mnt/c/dev/sml/doc/cppnow-2017/example/type_erasure.cpp:1:
#/mnt/c/dev/boost_1_65_1/boost/type_erasure/detail/instantiate.hpp: In static member function ‘static void boost::type_erasure::detail::instantiate_concept0::apply(Concept*, Map*)’:
#/mnt/c/dev/boost_1_65_1/boost/type_erasure/detail/instantiate.hpp:90:17: error: typedef ‘concept_sequence’ locally defined but not used [-Werror=unused-local-typedefs]

#In file included from /mnt/c/dev/boost_1_65_1/boost/type_erasure/binding.hpp:31:0,
#                 from /mnt/c/dev/boost_1_65_1/boost/type_erasure/any.hpp:40,
#                 from /mnt/c/dev/sml/doc/cppnow-2017/example/type_erasure.cpp:1:
#/mnt/c/dev/boost_1_65_1/boost/type_erasure/detail/vtable.hpp: In member function ‘bool boost::type_erasure::detail::vtable_storage<>::operator==(const boost::type_erasure::detail::vtable_storage<>&) const’:
#/mnt/c/dev/boost_1_65_1/boost/type_erasure/detail/vtable.hpp:156:43: error: unused parameter ‘other’ [-Werror=unused-parameter]

# Modify in order to avoid the 3 warnings above
#if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # gcc
#  target_compile_options(type_erasure PRIVATE
#      "-Wno-pedantic"
#      "-Wno-unused-local-typedefs"
#      "-Wno-unused-parameter")
#endif()

if(DOC_AS_TEST)
  add_test(cpp_now_2017_god_object god_object)

  add_test(cpp_now_2017_static_polymorphism static_polymorphism)

  #add_test(cpp_now_2017_type_erasure type_erasure)
endif()
