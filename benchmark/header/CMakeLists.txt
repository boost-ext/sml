add_executable(header_euml euml.cpp)
add_test(benchmark_header_euml header_euml)

# sml/benchmark/complex/euml2.cpp:14:10: fatal error: boost/msm/front/euml2/euml2.hpp: No such file or directory
# add_executable(header_euml2 euml2.cpp)

add_executable(header_sc sc.cpp)
add_test(benchmark_header_sc header_sc)

add_executable(header_sml sml.cpp)
add_test(benchmark_header_sml header_sml)
