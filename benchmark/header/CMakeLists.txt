add_executable(header_euml euml.cpp)

# sml/benchmark/complex/euml2.cpp:14:10: fatal error: boost/msm/front/euml2/euml2.hpp: No such file or directory
message( WARNING "benchmark/header/euml2.cpp build is disabled because <BOOST_ROOT>/boost/msm/front/euml2/euml2.hpp do not exist")
# add_executable(header_euml2 euml2.cpp)

add_executable(header_sc sc.cpp)

add_executable(header_sml sml.cpp)
