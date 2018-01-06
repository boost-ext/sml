add_executable(connection_v1 connection_v1.cpp)

add_executable(connection_v2 connection_v2.cpp)

# In file included from /usr/include/c++/7/variant:35:0,
#                  from /mnt/c/dev/sml/doc/embo-2017/example/connection_v3.cpp:2:
# /usr/include/c++/7/bits/c++17_warning.h:32:2: error: #error This file requires compiler and library support for the ISO C++ 2017 standard. This support must be enabled with the -std=c++17 or -std=gnu++17 compiler options.
# compilation terminated due to -Wfatal-errors.
# add_executable(connection_v3 connection_v3.cpp)

add_executable(connection_v4 connection_v4.cpp)

if(DOC_AS_TEST)
  add_test(embo_2017_connection_v1 connection_v1)

  add_test(embo_2017_connection_v2 connection_v2)

  #add_test(embo_2017_connection_v3 connection_v3)

  add_test(embo_2017_connection_v4 connection_v4)
endif()
