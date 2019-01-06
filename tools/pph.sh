#!/bin/bash
#
# Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
pph() {
  version=$1 revision=$2 patch=$3
  echo "//"
  echo "// Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)"
  echo "//"
  echo "// Distributed under the Boost Software License, Version 1.0."
  echo "// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)"
  echo "//"
  echo "#ifndef BOOST_SML_HPP"
  echo "#define BOOST_SML_HPP"
  echo "#if (__cplusplus < 201305L && _MSC_VER < 1900)"
  echo "#error \"[Boost].SML requires C++14 support (Clang-3.4+, GCC-5.1+, MSVC-2015+)\""
  echo "#else"
  echo "#define BOOST_SML_VERSION ${version}'${revision}'${patch}"
  echo "#define BOOST_SML_NAMESPACE_BEGIN namespace boost { namespace sml { inline namespace v${version}_${revision}_${patch} {"
  echo "#define BOOST_SML_NAMESPACE_END }}}"
  echo "#if defined(__clang__)"
  echo "#define __BOOST_SML_UNUSED __attribute__((unused))"
  echo "#define __BOOST_SML_VT_INIT {}"
  echo "#define __BOOST_SML_ZERO_SIZE_ARRAY(...) __VA_ARGS__ _[0]"
  echo "#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...)"
  echo "#define __BOOST_SML_TEMPLATE_KEYWORD template"
  echo "#pragma clang diagnostic push"
  echo "#pragma clang diagnostic ignored \"-Wgnu-string-literal-operator-template\""
  echo "#pragma clang diagnostic ignored \"-Wzero-length-array\""
  echo "#elif defined(__GNUC__)"
  echo "#if !defined(__has_builtin)"
  echo "#define __has_builtin(...) 0"
  echo "#endif"
  echo "#define __BOOST_SML_UNUSED __attribute__((unused))"
  echo "#define __BOOST_SML_VT_INIT {}"
  echo "#define __BOOST_SML_ZERO_SIZE_ARRAY(...) __VA_ARGS__ _[0]"
  echo "#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...) __VA_ARGS__ ? __VA_ARGS__ : 1"
  echo "#define __BOOST_SML_TEMPLATE_KEYWORD template"
  echo "#pragma GCC diagnostic push"
  echo "#pragma GCC diagnostic ignored \"-Wpedantic\""
  echo "#elif defined(_MSC_VER)"
  echo "#define __has_builtin(...) __has_builtin##__VA_ARGS__"
  echo "#define __has_builtin__make_integer_seq(...) 1"
  echo "#define __BOOST_SML_UNUSED"
  echo "#define __BOOST_SML_VT_INIT"
  echo "#define __BOOST_SML_ZERO_SIZE_ARRAY(...)"
  echo "#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...) __VA_ARGS__ ? __VA_ARGS__ : 1"
  echo "#if (_MSC_VER >= 1910) // MSVC 2017"
  echo "#define __BOOST_SML_TEMPLATE_KEYWORD template "
  echo "#else"
  echo "#define __BOOST_SML_TEMPLATE_KEYWORD"
  echo "#endif"
  echo "#pragma warning(disable:4503)"
  echo "#pragma warning(disable:4200)"
  echo "#endif"
  rm -rf tmp && mkdir tmp && cp -r boost tmp && cd tmp
  find . -iname "*.hpp" | xargs sed -i "s/\(.*\)__pph__/\/\/\/\/\1/g"
  find . -iname "*.hpp" | xargs sed -i "s/.*\(clang-format.*\)/\/\/\/\/\1/g"
  echo '
    BOOST_SML_NAMESPACE_BEGIN
    #include "boost/sml/state_machine.hpp"
    #include "boost/sml/transition_table.hpp"' > tmp.hpp
  cpp -C -P -nostdinc -I. tmp.hpp 2>/dev/null | \
    sed "s/\/\/\/\///" | \
    sed "s/[ \t$]*#[ \t]*define/##define/g" | \
    cpp -P -I. -fpreprocessed - 2>/dev/null | \
    sed "s/clang-format\(.*\)/\/\/ clang-format\1/g" | \
    sed "s/^##define/#define/g"
  cd .. && rm -rf tmp
  echo "BOOST_SML_NAMESPACE_END"
  echo "#undef __BOOST_SML_UNUSED"
  echo "#undef __BOOST_SML_VT_INIT"
  echo "#undef __BOOST_SML_ZERO_SIZE_ARRAY"
  echo "#undef __BOOST_SML_ZERO_SIZE_ARRAY_CREATE"
  echo "#undef __BOOST_SML_TEMPLATE_KEYWORD"
  echo "#if defined(__clang__)"
  echo "#pragma clang diagnostic pop"
  echo "#elif defined(__GNUC__)"
  echo "#undef __has_builtin"
  echo "#pragma GCC diagnostic pop"
  echo "#elif defined(_MSC_VER)"
  echo "#undef __has_builtin"
  echo "#undef __has_builtin__make_integer_seq"
  echo "#endif"
  echo "#endif"
  echo "#endif"
  echo
}

set -e
cd ${0%/*}/../include && pph `head -1 ../doc/CHANGELOG.md  | sed "s/.*\[\(.*\)\].*/\1/" | tr '.' ' '` > "boost/sml.hpp"
${CLANG_FORMAT:=clang-format} -i "boost/sml.hpp"
