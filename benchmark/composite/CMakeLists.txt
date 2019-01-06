#
# Copyright (c) 2016-2019 Jean Davy
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
add_executable(composite_euml euml.cpp)
add_test(benchmark_composite_euml composite_euml)

# sml/benchmark/complex/euml2.cpp:14:10: fatal error: boost/msm/front/euml2/euml2.hpp: No such file or directory
# add_executable(composite_euml2 euml2.cpp)

add_executable(composite_sc sc.cpp)
add_test(benchmark_composite_sc composite_sc)

if (NOT IS_MSVC_2015)
	add_executable(composite_sml sml.cpp)
	add_test(benchmark_composite_sml composite_sml)
	#[[In file included from /mnt/c/dev/sml/benchmark/composite/sml.cpp:9:0:
	/mnt/c/dev/sml/include/boost/sml.hpp: In instantiation of ‘struct boost::sml::v1_1_0::concepts::composable<playing>’:
	/mnt/c/dev/sml/include/boost/sml.hpp:2242:35:   recursively required by substitution of ‘template<class T> struct boost::sml::v1_1_0::front::state_sm<T, typename boost::sml::v1_1_0::aux::enable_if<boost::sml::v1_1_0::concepts::composable<T>::value, void>::type> [with T = playing
	’
	/mnt/c/dev/sml/include/boost/sml.hpp:2242:35:   required from here
	/mnt/c/dev/sml/include/boost/sml.hpp:1334:8: error: ‘boost::sml::v1_1_0::concepts::composable<playing>’ has a field ‘boost::sml::v1_1_0::concepts::composable<playing>::<anonymous>’ whose type uses the anonymous namespace [-Werror=subobject-linkage]
	]]
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		target_compile_options(composite_sml PRIVATE
		    "-Wno-subobject-linkage"
		    "-Wno-fatal-errors")
	endif()
endif()
