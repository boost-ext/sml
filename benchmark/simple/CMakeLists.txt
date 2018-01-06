add_executable(simple_euml euml.cpp)
add_test(benchmark_simple_euml simple_euml)

# sml/benchmark/complex/euml2.cpp:14:10: fatal error: boost/msm/front/euml2/euml2.hpp: No such file or directory
# add_executable(simple_euml2 euml2.cpp)

add_executable(simple_sc sc.cpp)
add_test(benchmark_simple_sc simple_sc)

add_executable(simple_sml sml.cpp)
add_test(benchmark_simple_sml simple_sml)
