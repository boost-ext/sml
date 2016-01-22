//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/msm/msm.hpp>

namespace msm = boost::msm;

struct example {
  auto configure() const noexcept {
    using namespace msm;
    return make_transition_table();
  }
};

int main() { msm::sm<example> sm; }
