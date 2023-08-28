//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <deque>
#include <memory>
#include <string>
#include <vector>

namespace sml = boost::sml;

struct event1 {};
struct event2 {};
struct event3 {};
struct event4 {};

const auto state1 = sml::state<class state1>;
const auto state2 = sml::state<class state2>;
const auto state3 = sml::state<class state3>;
const auto state4 = sml::state<class state4>;
const auto state5 = sml::state<class state5>;
const auto state6 = sml::state<class state6>;

test defer_minimal = [] {
  const auto c = [] {
    using namespace sml;
    // clang-format off
      return make_transition_table(
       *state1 + event<event1> / defer,
        state1 + event<event2> = state2,
        state2 + event<event1> = X
      );
      // clang-format off
  };

  sml::sm<decltype(c), sml::defer_queue<std::deque>> sm{c};
  sm.process_event(event1());
  sm.process_event(event2());
  expect(sm.is(sml::X));
};

test defer_queue_check = [] {
  class A;
  class B;

  const auto c = [] {
      using namespace sml;
      // clang-format off
      return make_transition_table(
       *state1 + event<event1> / defer,
        state1 + event<event2> = state2,
        state2 + event<event1> = state<A>,
        state<A>   + event<event1> = state<B>
      );
      // clang-format off
  };

  sml::sm<decltype(c), sml::defer_queue<std::deque>> sm{c};
  sm.process_event(event1());
  sm.process_event(event2());
  expect(sm.is(sml::state<A>));
};

test defer_transition = [] {
  const auto c = [] {
      using namespace sml;
      // clang-format off
      return make_transition_table(
       *state1 + event<event1> / defer = state2,
        state2 + event<event1> = X
      );
      // clang-format off
  };

  sml::sm<decltype(c), sml::defer_queue<std::deque>> sm{c};
  sm.process_event(event1());
  expect(sm.is(sml::X));
};

test defer_and_transitions = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
       *state1 + event<event1> / defer,
        state1 + on_entry<_> / [this] { entries.emplace_back(1); },
        state2 + event<event1> / defer,
        state2 + on_entry<_> / [this] { entries.emplace_back(2); },
        state1 + event<event2> = state2,
        state2 + event<event2> = state3,
        state3 + event<event1> = state4,
        state3 + on_entry<_> / [this] { entries.emplace_back(3); },
        state4 + event<event1> = state5,
        state4 + on_entry<_> / [this] { entries.emplace_back(4); },
        state5 + event<event1> = state6,
        state5 + on_entry<_> / [this] { entries.emplace_back(5); },
        state6 + on_entry<_> / [this] { entries.emplace_back(6); }
      );
      // clang-format on
    }

    std::vector<int> entries;
  };

  sml::sm<c, sml::defer_queue<std::deque>> sm;
  const c& c_ = sm;
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
    c() {}

    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
       *state1 + event<event1> / defer,
        state1 + on_entry<_> / [this] { entries.emplace_back(1); },
        state1 + event<event2> = state2,
        state2 = state3,
        state2 + on_entry<_> / [this] { entries.emplace_back(2); },
        state3 + event<event1> = state4,
        state4 + on_entry<_> / [this] { entries.emplace_back(4); },
        state3 + on_entry<_> / [this] { entries.emplace_back(3); },
        state2 + event<event1> = state5,
        state5 + on_entry<_> / [this] { entries.emplace_back(5); }
      );
      // clang-format on
    }

    std::vector<int> entries;
  };

  sml::sm<c, sml::defer_queue<std::deque>> sm{c{}};
  const c& c_ = sm;
  sm.process_event(event1());
  sm.process_event(event2());

  expect(4 == c_.entries.size());
  expect(1 == c_.entries[0]);
  expect(2 == c_.entries[1]);
  expect(3 == c_.entries[2]);
  expect(4 == c_.entries[3]);
};

test defer_reaction = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        * state1 + on_entry<sml::initial> / [this] { calls += "s1e|"; }
        , state1 + event<event1> / [this](sml::back::defer<event2, event3> deferEvent) -> void {
            calls += "a1.begin|";
            deferEvent(event2{});
            deferEvent(event3{});
            calls += "a1.end|";
          } = state2
         , state2 + event<event2> / [this] { calls += "a2|"; }
         , state2 + event<event3> / [this] { calls += "a3|"; } = X
      );
      // clang-format on
    }

    std::string calls{};
  };

  sml::sm<c, sml::defer_queue<std::deque>> sm{};

  sm.process_event(event1());
  expect(sm.is(sml::X));

  const c& c_ = sm;
  expect("s1e|a1.begin|a1.end|a2|a3|" == c_.calls);
};

test defer_order = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        * state1 + event<event1> / defer
        , state1 + event<event2> / defer
        , state1 + event<event3> / defer
        , state1 + event<event4> = state2
        , state2 + event<event1> / defer
        , state2 + event<event3> / defer
        , state2 + event<event2> = state3
        , state3 + event<event1> = state4
        , state3 + event<event3> = state5
        , state4 + event<event4> = state6
        , state6 + event<event3> / [] { expect(false); } // event3 should be disposed
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::defer_queue<std::deque>> sm{};

  sm.process_event(event1());
  sm.process_event(event2());
  sm.process_event(event3());
  sm.process_event(event4());
  sm.process_event(event4());
  expect(sm.is(state6));
};

test defer_and_internal_process_events = [] {
  struct c {
    std::vector<int> calls;

    auto operator()() {
      using namespace sml;

      // clang-format off
      return make_transition_table(
        * state1 + event<event1> / defer = state2
        , state3 <= state2 + event<event1>
        , X <= state3
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::defer_queue<std::deque>> sm{};
  sm.process_event(event1{});
  expect(sm.is(sml::X));
};

test defer_queue_event_move_dtor = [] {
  struct e1 {
    std::shared_ptr<int> data;
  };

  struct c {
    auto operator()() {
      using namespace sml;

      // clang-format off
      return make_transition_table(
        * state1 + event<e1> / defer
        , state1 + event<event1> / defer
        , state1 + event<event2> = state2
        , state2 + event<e1> = state2
        , state2 + event<event1> / defer
      );
      // clang-format on
    }
  };

  auto e1data1 = std::make_shared<int>(1);
  auto e1data2 = std::make_shared<int>(2);
  auto e1data3 = std::make_shared<int>(3);
  auto e1data4 = std::make_shared<int>(4);

  sml::sm<c, sml::defer_queue<std::deque>> sm{};

  // place defer events on front and back halves of deque
  sm.process_event(e1{e1data1});
  sm.process_event(e1{e1data2});
  sm.process_event(event1{});
  sm.process_event(event1{});
  sm.process_event(event1{});
  sm.process_event(event1{});
  sm.process_event(e1{e1data3});
  sm.process_event(e1{e1data4});
  sm.process_event(event2{});

  expect(sm.is(state2));
  expect(e1data1.use_count() == 1);
  expect(e1data2.use_count() == 1);
  expect(e1data3.use_count() == 1);
  expect(e1data4.use_count() == 1);
};

test defer_and_action = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      auto action1 = [this]{ calls += "a1|"; };
      auto action2 = [this]{ calls += "a2|"; };
      auto action3 = [this]{ calls += "a3|"; };

      // clang-format off
      return make_transition_table(
        * state1 + event<event1> / (action1, defer, defer)
        , state1 + event<event2> / (action2, defer)
        , state1 + event<event3> = state2
        , state2 + event<event1> / action3
        , state2 + event<event2> = X
      );
      // clang-format on
    }

    std::string calls{};
  };

  sml::sm<c, sml::defer_queue<std::deque>> sm{};
  const c& c_ = sm;

  sm.process_event(event1());
  sm.process_event(event2());
  sm.process_event(event3());
  expect(sm.is(sml::X));
  expect("a1|a2|a3|a3|" == c_.calls);
};

test defer_multi = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        * state1 + event<event1> / defer = state2
        , state2 + event<event1> / defer = state3
        , state3 + event<event1> / defer = state4
        , state4 + event<event1> = X
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::defer_queue<std::deque>> sm{};
  sm.process_event(event1());
  expect(sm.is(sml::X));
};