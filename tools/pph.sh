#!/bin/bash
#
# Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
pph() {
  version=$1 revision=$2 patch=$3
  echo "//"
  echo "// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)"
  echo "//"
  echo "// Distributed under the Boost Software License, Version 1.0."
  echo "// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)"
  echo "//"
  echo "#pragma once"
  echo "#if (__cplusplus < 201305L && _MSC_VER < 1900)"
  echo "#error \"Boost.MSM-lite requires C++14 support (Clang-3.4+, GCC-5.1+, MSVC-2015+)\""
  echo "#else"
  echo "#define BOOST_MSM_LITE_VERSION ${version}'${revision}'${patch}"
  echo "#define BOOST_MSM_LITE_NAMESPACE_BEGIN namespace boost { namespace msm { namespace lite { inline namespace v${version}_${revision}_${patch} {"
  echo "#define BOOST_MSM_LITE_NAMESPACE_END }}}}"
  echo "#if !defined(__has_builtin)"
  echo "#define __has_builtin(...) 0"
  echo "#endif"
  echo "#if !defined(__has_extension)"
  echo "#define __has_extension(...) 0"
  echo "#endif"
  echo "#if defined(__clang__)"
  echo "#pragma clang diagnostic push"
  echo "#pragma clang diagnostic ignored \"-Wgnu-string-literal-operator-template\""
  echo "#pragma clang diagnostic ignored \"-Wzero-length-array\""
  echo "#elif defined(__GNUC__)"
  echo "#pragma GCC diagnostic push"
  echo "#pragma GCC diagnostic ignored \"-Wpedantic\""
  echo "#endif"
  rm -rf tmp && mkdir tmp && cp -r boost tmp && cd tmp
  find . -iname "*.hpp" | xargs sed -i "s/\(.*\)__pph__/\/\/\/\/\1/g"
  find . -iname "*.hpp" | xargs sed -i "s/.*\(clang-format.*\)/\/\/\/\/\1/g"
  echo '
    BOOST_MSM_LITE_NAMESPACE_BEGIN
    #include "boost/msm-lite/back/state_machine.hpp"
    #include "boost/msm-lite/front/transition_table.hpp"' > tmp.hpp
  cpp -C -P -nostdinc -I. -DBOOST_DI_INJECT_HPP tmp.hpp 2>/dev/null | \
    sed "s/\/\/\/\///" | \
    sed "s/[ $]*#define/##define/g" | \
    cpp -P -I. -fpreprocessed - 2>/dev/null | \
    sed "s/clang-format\(.*\)/\/\/ clang-format\1/g" | \
    sed "s/^##define/#define/g"
  cd .. && rm -rf tmp
  echo "BOOST_MSM_LITE_NAMESPACE_END"
  echo "#endif"
  echo "#if defined(__clang__)"
  echo "#pragma clang diagnostic pop"
  echo "#elif defined(__GNUC__)"
  echo "#pragma GCC diagnostic pop"
  echo "#endif"
  echo
}

set -e
cd ${0%/*}/../include && pph `head -1 ../doc/CHANGELOG.md  | sed "s/.*\[\(.*\)\].*/\1/" | tr '.' ' '` > "boost/msm-lite.hpp"
${CLANG_FORMAT:=clang-format} -i "boost/msm-lite.hpp"
