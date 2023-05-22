//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cassert>
#include <deque>
#include <queue>

namespace sml = boost::sml;

namespace {
struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};

struct defer_and_process {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
       *"idle"_s + event<e1> / defer
      , "idle"_s + event<e2> = "s1"_s
      , "s1"_s   + event<e1> / process(e2{}) = "s2"_s
      , "s2"_s   + event<e3> / process(e4{})
      , "s2"_s   + event<e4> = X
    );
    // clang-format on
  }
};
}  // namespace

int main() {
  using namespace sml;
  sm<defer_and_process, sml::defer_queue<std::deque>, sml::process_queue<std::queue>>
      sm;  /// defer_queue policy to enable deferred events using std::queue
  assert(sm.is("idle"_s));

  assert(sm.process_event(e1{}));
  assert(sm.is("idle"_s));

  assert(!sm.process_event(e2{}));  /// triggers idle -> s1 and s1 -> s2 (via deferred e1)
                                    /// additionally triggers e2 again which is unhandled so it returns false
  assert(sm.is("s2"_s));

  assert(sm.process_event(e3{}));  /// triggers s2.process(e4) -> X (via processed e4)
  assert(sm.is(sml::X));
}
