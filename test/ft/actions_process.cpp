//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};

const auto idle = sml::state<class idle>;
const auto s1 = sml::state<class s1>;
const auto s2 = sml::state<class s2>;
const auto s3 = sml::state<class s3>;

test process_event = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> / process(e3{})
        , s1 + event<e3> / [this] { a_called++; } = X
      );
      // clang-format on
    }

    int a_called = 0;
  };

  sml::sm<c, sml::process_queue<std::queue>> sm{};
  c& c_ = sm;
  expect(sm.is(idle));
  expect(!sm.process_event(e4{}));
  expect(sm.process_event(e1{}));
  expect(sm.is(s1));
  expect(!c_.a_called);
  expect(sm.process_event(e2{}));  // + process(e3{})
  expect(1 == c_.a_called);
  expect(sm.is(sml::X));
};

test process_event_from_substate = [] {
  struct sub {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *s1 + event<e1> = s2
        , s2 + event<e2> / process(e3{})
        , s2 + event<e3> / process(e2{}) = s3
        , s3 + event<e3> = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = state<sub>
        , state<sub> + event<e4> / [this] { a_called++; }
      );
      // clang-format on
    }

    int a_called = 0;
  };

  sml::sm<c, sml::process_queue<std::queue>> sm;
  const c& c_ = sm;
  expect(sm.is(idle));
  expect(sm.is<decltype(sml::state<sub>)>(s1));

  expect(sm.process_event(e1{}));
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(s1));
  expect(!c_.a_called);

  expect(sm.process_event(e1{}));
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(s2));
  expect(!c_.a_called);

  expect(!sm.process_event(e2{}));  // + process(e3{}) + process(e2{})
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(s3));
  expect(!c_.a_called);

  expect(sm.process_event(e3{}));
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(sml::X));

  expect(sm.process_event(e4{}));
  expect(1 == c_.a_called);
};

test queue_process_events = [] {
  struct c {
    std::vector<int> calls;

    auto operator()() {
      using namespace sml;

      const auto a0 = [this] { calls.push_back(0); };
      const auto a1 = [this] { calls.push_back(1); };
      const auto a2 = [this] { calls.push_back(2); };

      // clang-format off
      return make_transition_table(
        * idle + on_entry<sml::initial> / (process(e1()), process(e2()), a0)
        , idle + event<e1> / a1
        , idle + event<e2> / a2 = X
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<std::queue>> sm{};
  expect(sm.is(sml::X));

  const c& c_ = sm;
  expect(3u == c_.calls.size());
  expect(0 == c_.calls[0]);
  expect(1 == c_.calls[1]);
  expect(2 == c_.calls[2]);
};

test queue_and_internal_process_events = [] {
  struct c {
    auto operator()() {
      using namespace sml;

      // clang-format off
      return make_transition_table(
        * idle + on_entry<sml::initial> / process(e1())
        , s1 <= idle + event<e1>
        , s2 <= s1 / process(e1())  // This process is unhandled, but the state machine still transitions to s2
        , s2 + on_entry<_> / process(e2())
        , s3 <= s2 + event<e2>
        , X <= s3
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<std::queue>> sm{};
  expect(sm.is(sml::X));
};

test queue_process_reaction = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        * s1 + on_entry<sml::initial> / [this] { calls += "s1e|"; }
        , s1 + event<e1> / [this](sml::back::process<e2, e3> processEvent) -> void {
            calls += "a1.begin|";
            processEvent(e2{});
            processEvent(e3{});
            calls += "a1.end|";
          }
         , s1 + event<e2> / [this] { calls += "a2|"; }
         , s2 + event<e3> / [this] { calls += "a3|"; }
         , s1 + unexpected_event<e3> / [this] { calls += "err|"; } = X
      );
      // clang-format on
    }

    std::string calls{};
  };

  sml::sm<c, sml::process_queue<std::queue>> sm{};

  expect(!sm.process_event(e1()));  // e3 was unexpected so it returns false
  expect(sm.is(sml::X));

  const c& c_ = sm;
  expect("s1e|a1.begin|a1.end|a2|err|" == c_.calls);
};

test mix_process_and_internal = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        * s1 + event<e1> / process(e2{})
        , s1 + event<e2> = s2
        , s2 = X
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<std::queue>> sm{};

  expect(sm.process_event(e1{}));
  expect(sm.is(sml::X));
};

test process_event_sent_from_substate = [] {
  struct sub {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *s1 + event<e1> / process(e2{}) = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = state<sub>
        , state<sub> + event<e2> = X
        ,*s2 + event<e2> = X
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<std::queue>> sm;
  expect(sm.is(idle, s2));
  expect(sm.is<decltype(sml::state<sub>)>(s1));

  expect(sm.process_event(e1{}));
  expect(sm.is(sml::state<sub>, s2));
  expect(sm.is<decltype(sml::state<sub>)>(s1));

  expect(sm.process_event(e1{}));  // + process(e2{})
  expect(sm.is(sml::X, sml::X));
  expect(sm.is<decltype(sml::state<sub>)>(sml::X));
};

test process_event_of_substate = [] {
  struct sub {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *s1 + event<e3> = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = state<sub>
        , state<sub> + event<e2> / process(e3{})
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<std::queue>> sm;
  expect(sm.is(idle));
  expect(sm.is<decltype(sml::state<sub>)>(s1));

  expect(sm.process_event(e1{}));
  expect(sm.is(sml::state<sub>));

  expect(sm.process_event(e2{}));  // + process(e2{})
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(sml::X));
};

test process_between_substates = [] {
  struct sub1 {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *s1 + event<e1> / process(e2{}) = X
      );
      // clang-format on
    }
  };

  struct sub2 {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *s2 + event<e2> / process(e3{}) = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *state<sub1> + event<e3> / process(e4{}) = X,
         *state<sub2> + event<e4> = X
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<std::queue>> sm;
  expect(sm.is(sml::state<sub1>, sml::state<sub2>));
  expect(sm.is<decltype(sml::state<sub1>)>(s1));
  expect(sm.is<decltype(sml::state<sub2>)>(s2));

  expect(sm.process_event(e1{}));
  expect(sm.is(sml::X, sml::X));
  expect(sm.is<decltype(sml::state<sub1>)>(sml::X));
  expect(sm.is<decltype(sml::state<sub2>)>(sml::X));
};
