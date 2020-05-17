//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <map>
#include <string>
#include <type_traits>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};

const auto idle = sml::state<class idle>;
const auto s1 = sml::state<class s1>;
const auto is_handled = sml::state<class is_handled>;
const auto errors = sml::state<class errors>;

test unexpected_event_empty = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *idle + unexpected_event<e1> = X
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
         (*idle) + event<e1> / []{},
           idle  + unexpected_event<e2> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  using namespace sml;
  sm.process_event(e1{});
  expect(sm.is(idle));
  sm.process_event(e2{});
  expect(sm.is(X));
};

test unexpected_specific_event = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        (*idle)      + event<e1> = is_handled,
          is_handled + unexpected_event<e1> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  using namespace sml;
  sm.process_event(e1{});
  expect(sm.is(is_handled));
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
         *(idle)      + event<e1> = is_handled,
           is_handled + unexpected_event<event_data>
              / [](const event_data& e) { expect(42 == e.i); } = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  using namespace sml;
  sm.process_event(e1{});
  expect(sm.is(is_handled));
  sm.process_event(event_data{42});
  expect(sm.is(X));
};

template <class TCalls, class T>
struct handle_unexpected_events {
  template <class TEvent>
  void operator()(const TEvent&, int& i) {
    expect(std::is_same<TEvent, T>::value);
    expect(42 == i);
    ++ue_calls[TCalls::unexpected_event_any];
  }
  std::map<TCalls, int>& ue_calls;
};

test unexpected_any_event = [] {
  enum class calls { unexpected_event_e1, unexpected_event_e2, unexpected_event_any };
  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        (*idle)      + event<e1> = is_handled,
          is_handled + unexpected_event<e1> / [this] { ++ue_calls[calls::unexpected_event_e1]; },
          is_handled + unexpected_event<e2> / [this](int& i) { i = 42; ++ue_calls[calls::unexpected_event_e2]; },
          is_handled + unexpected_event<_>  / handle_unexpected_events<calls, e3>{ue_calls} = X
      );
      // clang-format on
    }

    std::map<calls, int> ue_calls;
  };

  int i = 0;
  sml::sm<c> sm{i};
  c& c_ = sm;
  using namespace sml;
  sm.process_event(e1{});
  expect(sm.is(is_handled));

  sm.process_event(e1{});
  expect(1 == c_.ue_calls[calls::unexpected_event_e1]);
  expect(0 == c_.ue_calls[calls::unexpected_event_e2]);
  expect(0 == c_.ue_calls[calls::unexpected_event_any]);
  expect(sm.is(is_handled));

  sm.process_event(e1{});
  expect(2 == c_.ue_calls[calls::unexpected_event_e1]);
  expect(0 == c_.ue_calls[calls::unexpected_event_e2]);
  expect(0 == c_.ue_calls[calls::unexpected_event_any]);
  expect(sm.is(is_handled));

  sm.process_event(e2{});
  expect(2 == c_.ue_calls[calls::unexpected_event_e1]);
  expect(1 == c_.ue_calls[calls::unexpected_event_e2]);
  expect(0 == c_.ue_calls[calls::unexpected_event_any]);
  expect(sm.is(is_handled));

  sm.process_event(e1{});
  expect(3 == c_.ue_calls[calls::unexpected_event_e1]);
  expect(1 == c_.ue_calls[calls::unexpected_event_e2]);
  expect(0 == c_.ue_calls[calls::unexpected_event_any]);
  expect(sm.is(is_handled));

  sm.process_event(e3{});
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
        *(idle)   + event<e1> = s1,
        *(errors) + unexpected_event<e2> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  using namespace sml;

  sm.process_event(e1{});
  expect(sm.is(s1, errors));

  sm.process_event(e1{});
  expect(sm.is(s1, errors));

  sm.process_event(e2{});
  expect(sm.is(s1, X));
};
