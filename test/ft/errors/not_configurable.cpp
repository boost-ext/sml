//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct e1 {};

struct not_configurable {
  auto setup() const noexcept {  // should be configure
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *"idle"_s + event<e1> = X
    );
    // clang-format on
  }
};

int main() {
  sml::sm<not_configurable> sm;
  (void)sm;
}
