//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <map>
#include <string>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};

test unexpected_event_empty = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *"idle"_s  + unexpected_event<e1> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

test unexpected_specific_initial_state = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *("idle"_s) + event<e1> / []{},
           "idle"_s  + unexpected_event<e2> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  using namespace sml;
  sm.process_event(e1{});
  expect(sm.is("idle"_s));
  sm.process_event(e2{});
  expect(sm.is(X));
};

test unexpected_specific_event = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *("idle"_s)   + event<e1> = "handled"_s,
          "handled"_s + unexpected_event<e1> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  using namespace sml;
  sm.process_event(e1{});
  expect(sm.is("handled"_s));
  sm.process_event(e1{});
  expect(sm.is(X));
};

test unexpected_specific_event_with_data = [] {
  struct event_data {
    int i;
  };
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *("idle"_s)   + event<e1> = "handled"_s,
           "handled"_s + unexpected_event<event_data>
              / [](const auto& event) { expect(42 == event.i); } = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  using namespace sml;
  sm.process_event(e1{});
  expect(sm.is("handled"_s));
  sm.process_event(event_data{42});
  expect(sm.is(X));
};

test unexpected_any_event = [] {
  enum class calls { unexpected_event_e1, unexpected_event_e2, unexpected_event_any };
  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *("idle"_s)   + event<e1> = "handled"_s,
          "handled"_s + unexpected_event<e1> / [this] { ++ue_calls[calls::unexpected_event_e1]; },
          "handled"_s + unexpected_event<e2> / [this] { ++ue_calls[calls::unexpected_event_e2]; },
          "handled"_s + unexpected_event<> / [this] { ++ue_calls[calls::unexpected_event_any]; } = X
      );
      // clang-format on
    }

    std::map<calls, int> ue_calls;
  };

  c c_;
  sml::sm<c> sm{c_};
  using namespace sml;
  sm.process_event(e1{});
  expect(sm.is("handled"_s));

  sm.process_event(e1{});
  expect(1 == c_.ue_calls[calls::unexpected_event_e1]);
  expect(0 == c_.ue_calls[calls::unexpected_event_e2]);
  expect(0 == c_.ue_calls[calls::unexpected_event_any]);
  expect(sm.is("handled"_s));

  sm.process_event(e1{});
  expect(2 == c_.ue_calls[calls::unexpected_event_e1]);
  expect(0 == c_.ue_calls[calls::unexpected_event_e2]);
  expect(0 == c_.ue_calls[calls::unexpected_event_any]);
  expect(sm.is("handled"_s));

  sm.process_event(e2{});
  expect(2 == c_.ue_calls[calls::unexpected_event_e1]);
  expect(1 == c_.ue_calls[calls::unexpected_event_e2]);
  expect(0 == c_.ue_calls[calls::unexpected_event_any]);
  expect(sm.is("handled"_s));

  sm.process_event(e1{});
  expect(3 == c_.ue_calls[calls::unexpected_event_e1]);
  expect(1 == c_.ue_calls[calls::unexpected_event_e2]);
  expect(0 == c_.ue_calls[calls::unexpected_event_any]);
  expect(sm.is("handled"_s));

  sm.process_event(int{});
  expect(3 == c_.ue_calls[calls::unexpected_event_e1]);
  expect(1 == c_.ue_calls[calls::unexpected_event_e2]);
  expect(1 == c_.ue_calls[calls::unexpected_event_any]);
  expect(sm.is(X));
};

test unexpected_event_orthogonal_region = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *("idle"_s)   + event<e1> = "s1"_s,
        *("errors"_s) + unexpected_event<e2> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  using namespace sml;

  sm.process_event(e1{});
  expect(sm.is("s1"_s, "errors"_s));

  sm.process_event(e1{});
  expect(sm.is("s1"_s, "errors"_s));

  sm.process_event(e2{});
  expect(sm.is("s1"_s, X));
};
