//
// Copyright (c) 2016-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <common/test.hpp>

int main() {}

#include "fwd.hpp"

namespace sml = boost::sml;

struct fwd_sm {
  auto operator()() noexcept {
    using namespace sml;
    return make_transition_table(*"idle"_s + "event"_e = X);
  }
};

c::c() : sm{std::make_unique<sml::sm<fwd_sm>>()} {}

void c::update() {
  using namespace sml;
  sm->process_event("event"_e());
}

bool c::is_done() const { return sm->is(sml::X); }

test should_transition_to_terminate_state = [] {
  c c_{};
  expect(!c_.is_done());
  c_.update();
  expect(c_.is_done());
};
