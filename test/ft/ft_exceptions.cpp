//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/msm-lite.hpp>

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};
struct e3 {};

auto idle = msm::state<class idle>;
auto error = msm::state<class error>;
auto s1 = msm::state<class s1>;
auto s2 = msm::state<class s2>;

struct exception1 {};
struct exception2 {};
struct exception_data {
  int value = 0;
};

test exception_minimal = [] {
  struct c {
    auto operator()() const {
      using namespace msm;
      // clang-format off
      return make_transition_table(
       *idle + event<e1> / [] { throw exception1{}; } = s1
      , idle + exception<exception1> = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(msm::X));
};

test exception_data_minimal = [] {
  struct c {
    auto operator()() const {
      using namespace msm;
      auto guard = [](const auto& ex) { return ex.value == 42; };

      // clang-format off
      return make_transition_table(
       *idle + event<e1> / [] { throw exception_data{42}; } = s1
      , idle + exception<exception_data> [ guard ] = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(msm::X));
};

test exception_many = [] {
  struct c {
    auto operator()() const {
      using namespace msm;
      // clang-format off
      return make_transition_table(
       *idle + event<e1> / [] { throw exception2{}; } = s1
      , idle + exception<exception1> = s2
      , idle + exception<exception2> = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});  // throws exception2
  expect(sm.is(msm::X));
};

test generic_exception_handler = [] {
  struct c {
    auto operator()() const {
      using namespace msm;
      // clang-format off
      return make_transition_table(
       *idle + event<e1> / [] { throw int{}; } = s1
      , idle + exception<exception1> = s2
      , idle + exception<> = X // generic handler
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});  // throws int
  expect(sm.is(msm::X));
};

test generic_exception_handler_priority = [] {
  struct c {
    auto operator()() const {
      using namespace msm;
      // clang-format off
      return make_transition_table(
       *idle + event<e1> / [] { throw exception1{}; } = s1
      , idle + exception<exception1> = s2
      , idle + exception<> = X // generic handler
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(s2));
};

test exception_not_handled = [] {
  struct c {
    auto operator()() const {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *idle + event<e1> / [] { throw exception1{}; } = s1
      , *error + exception<> / [] {}
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});
  expect(sm.is(idle));
};

test exception_orthogonal_handler = [] {
  struct c {
    auto operator()() const {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *idle + event<e1> / [] { throw exception1{}; } = s1
      , *error + exception<exception1> = X
      , *error + exception<exception2> = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(idle, msm::X));
};

test exception_orthogonal_handler_generic_handler = [] {
  struct c {
    auto operator()() const {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *idle + event<e1> / [] { throw exception1{}; } = s1
      , *error + exception<> = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(idle, msm::X));
};

test exception_orthogonal_handler_generic_handler_with_exception = [] {
  struct c {
    auto operator()() const {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *idle + event<e1> / [] { throw exception1{}; } = s1
      , *error + exception<exception1> / [] { throw exception2{}; }
      , *error + exception<> = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(idle, msm::X));
};
