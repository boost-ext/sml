#
# Copyright (c) 2016-2019 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
add_example(header_euml benchmark_header_euml euml.cpp)

# sml/benchmark/complex/euml2.cpp:14:10: fatal error: boost/msm/front/euml2/euml2.hpp: No such file or directory
# add_executable(header_euml2 euml2.cpp)

add_example(header_sc benchmark_header_sc sc.cpp)

add_example(header_sml benchmark_header_sml sml.cpp)
