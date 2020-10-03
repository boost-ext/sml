//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct context {};

struct e1 {};
struct e2 {};
struct e3 {};

auto idle = sml::state<class idle>;
auto error = sml::state<class error>;
auto s1 = sml::state<class s1>;
auto s2 = sml::state<class s2>;

struct exception1 {};
struct exception2 {};
struct exception_data {
  int value = 0;
};

#if !defined(_MSC_VER)
test exception_minimal = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> / [] { throw exception1{}; } = s1
        , s1 + exception<exception1> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(sml::X));
};

test exception_no_transition = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> / [] { throw exception1{}; }
        , idle + exception<exception1> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(sml::X));
};

test exception_data_minimal = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      auto guard = [](const auto &ex) { return ex.value == 42; };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> / [] { throw exception_data{42}; } = s1
        , s1 + exception<exception_data> [ guard ] = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(sml::X));
};

test exception_many = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> / [] { throw exception2{}; } = s1
        , s1 + exception<exception1> = s2
        , s1 + exception<exception2> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e1{});  // throws exception2
  expect(sm.is(sml::X));
};

test generic_exception_handler = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> / [] { throw int{}; } = s1
        , s1 + exception<exception1> = s2
        , s1 + exception<_> = X // generic handler
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e1{});  // throws int
  expect(sm.is(sml::X));
};

test generic_exception_handler_priority = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> / [] { throw exception1{}; } = s1
        , s1 + exception<exception1> = s2
        , s1 + exception<_> = X // generic handler
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(s2));
};

test exception_not_handled = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> / [] { throw exception1{}; } = s1
       , *error + exception<_> / [] {}
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e1{});
  expect(sm.is(s1));
};

test exception_orthogonal_handler = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
          *idle + event<e1> / [] { throw exception1{}; } = s1
        , *error + exception<exception1> = X
        , *error + exception<exception2> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(s1, sml::X));
};

test exception_orthogonal_handler_generic_handler = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> / [] { throw exception1{}; } = s1
       , *error + exception<_> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(s1, sml::X));
};

test exception_orthogonal_handler_generic_handler_with_exception = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
          *idle + event<e1> / [] { throw exception1{}; } = s1
        , *error + exception<exception1> / [] { throw exception2{}; }
        , *error + exception<_> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(s1, sml::X));
};

test exception_and_unexpected_event = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + unexpected_event<e1> / [] { throw exception1{}; } = s1
       , *error + exception<_> / [] {}
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e1{});
  expect(sm.is(s1));
};

test exception_and_context = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
        return make_transition_table(
           *idle + event<e1> / [] { throw exception1{}; } = s1
          , s1 + exception<exception1> / [](const context &, context&, const auto &){} = X
        );
      // clang-format on
    }
  };

  context ctx{};
  sml::sm<c> sm{ctx};
  sm.process_event(e1{});  // throws exception1
  expect(sm.is(sml::X));
};

test propage_exception_if_no_handled = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
        return make_transition_table(
           *idle + event<e1> / [] { throw exception1{}; } = X
        );
      // clang-format on
    }
  };

  sml::sm<c> sm{};
  auto exception = false;
  try {
    sm.process_event(e1{});  // throws exception1
  } catch (const exception1 &) {
    expect(sm.is(sml::X));
    exception = true;
  }

  expect(exception);
};
#endif
