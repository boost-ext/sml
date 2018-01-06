# Visual Studio 15 2017 failure
# doc\cppnow-2016\example\at.cpp(158): error C2938: 'at_key_t<m,std::integral_constant<int,1>,void>' : Failed to specialize alias template
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_executable(at at.cpp)
  add_test(doc_cpp_now_2016_at at)

  add_executable(hana_at hana_at.cpp)
  add_test(doc_cpp_now_2016_hana_at hana_at)

  add_executable(hana_unique hana_unique.cpp)
  add_test(doc_cpp_now_2016_hana_unique hana_unique)

  add_executable(mpl_unique mpl_unique.cpp)
  add_test(doc_cpp_now_2016_mpl_unique mpl_unique)
endif()

add_executable(mpl_at mpl_at.cpp)
add_test(doc_cpp_now_2016_mpl_at mpl_at)

add_executable(unique unique.cpp)
add_test(doc_cpp_now_2016_unique unique)
