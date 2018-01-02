add_executable(complex_euml euml.cpp)

# sml/benchmark/complex/euml2.cpp:14:10: fatal error: boost/msm/front/euml2/euml2.hpp: No such file or directory
message( "<BOOST_ROOT>/boost/msm/front/euml2/euml2.hpp: No such file or directory")
message( "complex_euml2 build canceled")
# add_executable(complex_euml2 euml2.cpp)

add_executable(complex_sc sc.cpp)

add_executable(complex_sml sml.cpp)

add_executable(switch switch.cpp)

# In order to build variant, set CXX_STANDARD to 17
if (CMAKE_CXX_STANDARD EQUAL 17 OR CMAKE_CXX_STANDARD GREATER 17 ) # gcc
  add_executable(variant variant.cpp)
else()
  message( "variant requires C++ 17")
endif()
