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

const auto idle = sml::state<class idle>;
const auto s1 = sml::state<class s1>;
const auto handled = sml::state<class handled>;
const auto errors = sml::state<class errors>;

test unexpected_event_empty = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *idle  + unexpected_event<e1> = X
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
         *(idle) + event<e1> / []{},
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
        *(idle)   + event<e1> = handled,
          handled + unexpected_event<e1> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  using namespace sml;
  sm.process_event(e1{});
  expect(sm.is(handled));
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
         *(idle)   + event<e1> = handled,
           handled + unexpected_event<event_data>
              / [](const event_data& event) { expect(42 == event.i); } = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  using namespace sml;
  sm.process_event(e1{});
  expect(sm.is(handled));
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
        *(idle)   + event<e1> = handled,
          handled + unexpected_event<e1> / [this] { ++ue_calls[calls::unexpected_event_e1]; },
          handled + unexpected_event<e2> / [this] { ++ue_calls[calls::unexpected_event_e2]; },
          handled + unexpected_event<_>  / [this] { ++ue_calls[calls::unexpected_event_any]; } = X
      );
      // clang-format on
    }

    std::map<calls, int> ue_calls;
  };

  c c_;
  sml::sm<c> sm{c_};
  using namespace sml;
  sm.process_event(e1{});
  expect(sm.is(handled));

  sm.process_event(e1{});
  expect(1 == c_.ue_calls[calls::unexpected_event_e1]);
  expect(0 == c_.ue_calls[calls::unexpected_event_e2]);
  expect(0 == c_.ue_calls[calls::unexpected_event_any]);
  expect(sm.is(handled));

  sm.process_event(e1{});
  expect(2 == c_.ue_calls[calls::unexpected_event_e1]);
  expect(0 == c_.ue_calls[calls::unexpected_event_e2]);
  expect(0 == c_.ue_calls[calls::unexpected_event_any]);
  expect(sm.is(handled));

  sm.process_event(e2{});
  expect(2 == c_.ue_calls[calls::unexpected_event_e1]);
  expect(1 == c_.ue_calls[calls::unexpected_event_e2]);
  expect(0 == c_.ue_calls[calls::unexpected_event_any]);
  expect(sm.is(handled));

  sm.process_event(e1{});
  expect(3 == c_.ue_calls[calls::unexpected_event_e1]);
  expect(1 == c_.ue_calls[calls::unexpected_event_e2]);
  expect(0 == c_.ue_calls[calls::unexpected_event_any]);
  expect(sm.is(handled));

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
