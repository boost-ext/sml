//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm-lite.hpp"

namespace msm = boost::msm::lite;

struct runtime_event {};
struct e1 {};  // missing id

struct not_dispatchable {
  auto operator()() const noexcept {
    using namespace msm;
    // clang-format off
    return make_transition_table(
        *"idle"_s + event<e1> = X
    );
    // clang-format on
  }
};

int main() {
  msm::sm<not_dispatchable> sm;
  msm::make_dispatch_table<runtime_event, 1, 10>(sm);
}
