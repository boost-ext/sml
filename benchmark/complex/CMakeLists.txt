if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_executable(complex_euml euml.cpp)
  add_test(benchmark_complex_euml complex_euml)
endif()

# sml/benchmark/complex/euml2.cpp:14:10: fatal error: boost/msm/front/euml2/euml2.hpp: No such file or directory
# add_executable(complex_euml2 euml2.cpp)

add_executable(complex_sc sc.cpp)
add_test(benchmark_complex_sc complex_sc)

add_executable(complex_sml sml.cpp)
add_test(benchmark_complex_sml complex_sml)
#[[In file included from /mnt/c/dev/sml/benchmark/composite/sml.cpp:9:0:
/mnt/c/dev/sml/include/boost/sml.hpp: In instantiation of ‘struct boost::sml::v1_1_0::concepts::composable<playing>’:
/mnt/c/dev/sml/include/boost/sml.hpp:2242:35:   recursively required by substitution of
/mnt/c/dev/sml/include/boost/sml.hpp:2242:35:   required from here
/mnt/c/dev/sml/include/boost/sml.hpp:1334:8: error: ‘boost::sml::v1_1_0::concepts::composable<playing>’
    has a field ‘boost::sml::v1_1_0::concepts::composable<playing>::<anonymous>’
whose type uses the anonymous namespace [-Werror=subobject-linkage]]
target_compile_options(complex_sml PRIVATE
    "-Wno-subobject-linkage"
    "-Wno-fatal-errors")

add_executable(switch switch.cpp)
add_test(benchmark_switch switch)

if (${CMAKE_CXX_STANDARD} EQUAL 17 OR ${CMAKE_CXX_STANDARD} GREATER 17 ) # gcc
    #[[/mnt/c/dev/sml/benchmark/complex/variant.cpp:138:13:   required from here
    /usr/include/c++/7/variant:1242:44: error: no match for call to ‘(overload<>) (std::variant_alternative_t<0, std::variant<c::S1, c::S2, c::S3
    ]]
    #add_executable(variant variant.cpp)
endif()
