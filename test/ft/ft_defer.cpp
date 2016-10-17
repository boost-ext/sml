//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/msm-lite.hpp>
#include <queue>
#include <vector>

namespace msm = boost::msm::lite;

struct event1 {};
struct event2 {};

test defer_and_transitions = [] {
  struct c {
    auto operator()() {
      using namespace msm;
      // clang-format off
      return make_transition_table(
       *"state1"_s + event<event1> / defer,
        "state1"_s + on_entry / [this] { entries.emplace_back(1); },
        "state2"_s + event<event1> / defer,
        "state2"_s + on_entry / [this] { entries.emplace_back(2); },
        "state1"_s + event<event2> = "state2"_s,
        "state2"_s + event<event2> = "state3"_s,
        "state3"_s + event<event1> = "state4"_s,
        "state3"_s + on_entry / [this] { entries.emplace_back(3); },
        "state4"_s + event<event1> = "state5"_s,
        "state4"_s + on_entry / [this] { entries.emplace_back(4); },
        "state5"_s + event<event1> = "state6"_s,
        "state5"_s + on_entry / [this] { entries.emplace_back(5); },
        "state6"_s + on_entry / [this] { entries.emplace_back(6); }
      );
      // clang-format on
    }

    std::vector<int> entries;
  };

  c c_;
  msm::sm<c, msm::defer_queue<std::queue>> sm{c_};
  sm.process_event(event1());
  sm.process_event(event1());
  sm.process_event(event2());
  sm.process_event(event2());

  expect(5 == c_.entries.size());
  expect(1 == c_.entries[0]);
  expect(2 == c_.entries[1]);
  expect(3 == c_.entries[2]);
  expect(4 == c_.entries[3]);
  expect(5 == c_.entries[4]);
};

test defer_and_anonymous = [] {
  struct c {
    auto operator()() {
      using namespace msm;
      // clang-format off
      return make_transition_table(
       *"state1"_s + event<event1> / defer,
        "state1"_s + on_entry / [this] { entries.emplace_back(1); },
        "state1"_s + event<event2> = "state2"_s,
        "state2"_s  = "state3"_s,
        "state2"_s + on_entry / [this] { entries.emplace_back(2); },
        "state3"_s + event<event1> = "state4"_s,
        "state4"_s + on_entry / [this] { entries.emplace_back(4); },
        "state3"_s + on_entry / [this] { entries.emplace_back(3); },
        "state2"_s + event<event1> = "state5"_s,
        "state5"_s + on_entry / [this] { entries.emplace_back(5); }
      );
      // clang-format on
    }

    std::vector<int> entries;
  };

  c c_;
  msm::sm<c, msm::defer_queue<std::queue>> sm{c_};
  sm.process_event(event1());
  sm.process_event(event2());

  expect(4 == c_.entries.size());
  expect(1 == c_.entries[0]);
  expect(2 == c_.entries[1]);
  expect(3 == c_.entries[2]);
  expect(4 == c_.entries[3]);
};
