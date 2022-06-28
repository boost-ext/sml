//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cstdio>

namespace sml = boost::sml;

struct connect {};
struct established {};
struct ping {};
struct disconnect {};
struct timeout {};

// clang-format off
struct dispatch_policy {
  auto operator()() const {
    const auto establish = []{ std::puts("establish"); };
    const auto close = []{ std::puts("close"); };
    const auto is_valid = [](auto const&) { return true; };
    const auto reset_timeout = []{ std::puts("reset_timeout"); };

    using namespace sml;
    return make_transition_table(
      * "Disconnected"_s + event<connect> / establish                = "Connecting"_s,
        "Connecting"_s   + event<established>                        = "Connected"_s,
        "Connected"_s    + event<ping> [ is_valid ] / reset_timeout,
        "Connected"_s    + event<timeout> / establish                = "Connecting"_s,
        "Connected"_s    + event<disconnect> / close                 = "Disconnected"_s
    );
  }
};
// clang-format on

int main() {
  {
    sml::sm<dispatch_policy, sml::dispatch<sml::back::policies::jump_table>> sm{};
    sm.process_event(connect{});
    sm.process_event(established{});
    sm.process_event(ping{});
    sm.process_event(disconnect{});
    sm.process_event(connect{});
    sm.process_event(established{});
    sm.process_event(ping{});
  }

  {
    sml::sm<dispatch_policy, sml::dispatch<sml::back::policies::branch_stm>> sm{};
    sm.process_event(connect{});
    sm.process_event(established{});
    sm.process_event(ping{});
    sm.process_event(disconnect{});
    sm.process_event(connect{});
    sm.process_event(established{});
    sm.process_event(ping{});
  }

  {
    sml::sm<dispatch_policy, sml::dispatch<sml::back::policies::switch_stm>> sm{};
    sm.process_event(connect{});
    sm.process_event(established{});
    sm.process_event(ping{});
    sm.process_event(disconnect{});
    sm.process_event(connect{});
    sm.process_event(established{});
    sm.process_event(ping{});
  }

#if defined(__cpp_fold_expressions)
  {
    sml::sm<dispatch_policy, sml::dispatch<sml::back::policies::fold_expr>> sm{};
    sm.process_event(connect{});
    sm.process_event(established{});
    sm.process_event(ping{});
    sm.process_event(disconnect{});
    sm.process_event(connect{});
    sm.process_event(established{});
    sm.process_event(ping{});
  }
#endif
}
