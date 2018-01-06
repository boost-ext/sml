add_executable(simple_euml euml.cpp)

# sml/benchmark/complex/euml2.cpp:14:10: fatal error: boost/msm/front/euml2/euml2.hpp: No such file or directory
message( WARNING "benchmark/simple/euml2.cpp build is disabled because <BOOST_ROOT>/boost/msm/front/euml2/euml2.hpp do not exist")
# add_executable(simple_euml2 euml2.cpp)

add_executable(simple_sc sc.cpp)

add_executable(simple_sml sml.cpp)
