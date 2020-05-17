//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct runtime_event {};
struct e1 {};  // missing id

struct not_dispatchable {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *"idle"_s + event<e1> = X
    );
    // clang-format on
  }
};

int main() {
  sml::sm<not_dispatchable> sm;
  sml::make_dispatch_table<runtime_event, 1, 10>(sm);
}
