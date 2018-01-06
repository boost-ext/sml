add_executable(at at.cpp)

add_executable(hana_at hana_at.cpp)

add_executable(hana_unique hana_unique.cpp)

add_executable(mpl_at mpl_at.cpp)

add_executable(mpl_unique mpl_unique.cpp)

add_executable(unique unique.cpp)

set(DOC_AS_TEST ON)
message("DOC_AS_TEST=${DOC_AS_TEST}")
if(DOC_AS_TEST)
  message("DOC_AS_TEST=${DOC_AS_TEST}")
  add_test(cpp_now_2016_at at)

  add_test(cpp_now_2016_hana_at hana_at)

  add_test(cpp_now_2016_hana_unique hana_unique)

  add_test(cpp_now_2016_mpl_at mpl_at)

  add_test(cpp_now_2016_mpl_unique mpl_unique)

  add_test(cpp_now_2016_unique unique)
endif()