//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm-lite.hpp"

namespace msm = boost::msm::lite;

struct e1 {};

struct not_configurable {
  auto setup() const noexcept {  // should be configure
    using namespace msm;
    // clang-format off
    return make_transition_table(
        *"idle"_s + event<e1> = X
    );
    // clang-format on
  }
};

int main() {
  msm::sm<not_configurable> sm;
  (void)sm;
}
