//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/msm-lite.hpp>
#include <string>
#include <utility>

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};
struct e6 {};

const auto idle = msm::state<class idle>;
const auto idle2 = msm::state<class idle2>;
const auto s1 = msm::state<class s1>;
const auto s2 = msm::state<class s2>;
const auto s3 = msm::state<class s3>;

test composite = [] {
  static auto guard = [](int i) {
    expect(42 == i);
    return true;
  };

  struct guard2 {
    bool operator()(double d) const {
      expect(87.0 == d);
      return true;
    }
  };

  struct sub {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e3> / [this] { a_in_sub++; } = s1
        , s1 + event<e4> / [this] { a_in_sub++; } = s2
      );
      // clang-format on
    }

    int a_in_sub = 0;
  };

  struct c {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [guard2{}] / [this] { a_initial = true; } = s1
        , s1 + event<e2> [guard]  / [this]{ a_enter_sub_sm = true; } = state<sub>
        , state<sub> + msm::on_entry / [this] { a_on_entry_sub_sm = true; }
        , state<sub> + msm::on_exit / [this] { a_on_exit_sub_sm = true; }
        , state<sub> + event<e5> [guard2{}] / [this] { a_exit_sub_sm = true; } = s2
      );
      // clang-format on
    }

    bool a_initial = false;
    bool a_enter_sub_sm = false;
    bool a_exit_sub_sm = false;
    bool a_on_exit_sub_sm = false;
    bool a_on_entry_sub_sm = false;
  };

  c c_;
  sub sub_;
  msm::sm<c> sm{c_, sub_, 87.0, 42};

  expect(sm.is(idle));
  sm.process_event(e1());
  expect(c_.a_initial);

  sm.process_event(e2());
  expect(c_.a_enter_sub_sm);
  expect(c_.a_on_entry_sub_sm);
  expect(0 == sub_.a_in_sub);

  sm.process_event(e3());
  expect(1 == sub_.a_in_sub);

  sm.process_event(e4());
  expect(2 == sub_.a_in_sub);
  sm.process_event(e5());
  expect(2 == sub_.a_in_sub);
  expect(c_.a_on_entry_sub_sm);
  expect(c_.a_exit_sub_sm);
  expect(sm.is(s2));
};

test composite_def_ctor = [] {
  static auto in_sub = 0;

  struct sub {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e3> / [] { in_sub++; } = s1
        , s1 + event<e4> / [] { in_sub++; } = s2
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = state<sub>
        , state<sub> + event<e5> = s2
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;

  expect(sm.is(idle));
  sm.process_event(e1());

  sm.process_event(e2());
  expect(0 == in_sub);

  sm.process_event(e3());
  expect(1 == in_sub);

  sm.process_event(e4());
  expect(2 == in_sub);
  sm.process_event(e5());
  expect(2 == in_sub);
  expect(sm.is(s2));
};

test composite_transition_the_same_event = [] {
  struct sub {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = s2
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = state<sub>
        , state<sub> + event<e3> = "s1"_s
        , "s1"_s + event<e4> = state<sub>
        , state<sub> + event<e5> = X
      );
      // clang-format on
    }
  };

  using namespace msm;

  c c_;
  sub sub_;
  msm::sm<c> sm{c_, sub_};

  expect(sm.is(idle));
  sm.process_event(e1());
  expect(sm.is(state<sub>));
  // expect(subsm.is(idle));

  sm.process_event(e4());

  sm.process_event(e1());
  expect(sm.is(state<sub>));
  // expect(subsm.is(s1));

  sm.process_event(e2());
  expect(sm.is(state<sub>));
  // expect(subsm.is(s2));

  sm.process_event(e3());
  expect(sm.is("s1"_s));
  // expect(subsm.is(s2));

  sm.process_event(e4());
  expect(sm.is(state<sub>));
  // expect(subsm.is(idle));  // no history
};

test composite_history = [] {
  struct sub {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(H) + event<e1> = s1
        , s1 + event<e2> = s2
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = state<sub>
        , state<sub> + event<e3> = "s1"_s
        , "s1"_s + event<e4> = state<sub>
        , state<sub> + event<e5> = X
      );
      // clang-format on
    }
  };

  using namespace msm;

  c c_;
  sub sub_;
  msm::sm<c> sm{c_, sub_};

  expect(sm.is(idle));
  sm.process_event(e1());
  expect(sm.is(state<sub>));
  // expect(subsm.is(idle));

  sm.process_event(e4());

  sm.process_event(e1());
  expect(sm.is(state<sub>));
  // expect(subsm.is(s1));

  sm.process_event(e2());
  expect(sm.is(state<sub>));
  // expect(subsm.is(s2));

  sm.process_event(e3());
  expect(sm.is("s1"_s));
  // expect(subsm.is(s2));

  sm.process_event(e4());
  expect(sm.is(state<sub>));
  // expect(subsm.is(s2));  // history
};

test composite_history_region = [] {
  struct sub {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
          idle(H) + event<e1> = s1
        , s1 + event<e2> = s2
        //-----------------------//
        , *idle2 + event<e2> = s3
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = state<sub>
        , state<sub> + event<e3> = "s1"_s
        , "s1"_s + event<e4> = state<sub>
        , state<sub> + event<e5> = X
      );
      // clang-format on
    }
  };

  using namespace msm;

  // msm::sm<sub> subsm;
  msm::sm<c> sm;

  expect(sm.is(idle));
  sm.process_event(e1());
  expect(sm.is(state<sub>));
  // expect(subsm.is(idle, idle2));

  sm.process_event(e4());

  sm.process_event(e1());
  expect(sm.is(state<sub>));
  // expect(subsm.is(s1, idle2));

  sm.process_event(e2());
  expect(sm.is(state<sub>));
  // expect(subsm.is(s2, s3));

  sm.process_event(e3());
  expect(sm.is("s1"_s));
  // expect(subsm.is(s2, s3));

  sm.process_event(e4());
  expect(sm.is(state<sub>));
  // expect(subsm.is(s2, idle2));  // history for region 1
};

test composite_custom_ctor = [] {
  static auto in_sub = 0;

  struct sub {
    explicit sub(int i) : i(i) {}

    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e3> / [this] { in_sub+=i; } = s1
        , s1 + event<e4> / [this] { in_sub+=i; } = s2
      );
      // clang-format on
    }
    int i = 0;
  };

  struct c {
    auto operator()() noexcept {
      using namespace msm;

      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = state<sub>
        , state<sub> + event<e5> = s2
      );
      // clang-format on
    }
  };

  constexpr auto i = 2;
  auto test = [=](auto &&sm) {
    expect(sm.is(idle));
    sm.process_event(e1());

    sm.process_event(e2());
    expect(0 == in_sub);

    sm.process_event(e3());
    expect(1 * i == in_sub);

    sm.process_event(e4());
    expect(2 * i == in_sub);
    sm.process_event(e5());
    expect(2 * i == in_sub);
    expect(sm.is(s2));
  };

  {
    in_sub = 0;
    sub sub_{i};
    msm::sm<c> sm{sub_};
    test(static_cast<decltype(sm) &&>(sm));
  }

  {
    in_sub = 0;
    sub sub_{i};
    msm::sm<c> sm{sub_};
    test(static_cast<decltype(sm) &&>(sm));
  }

  {
    in_sub = 0;
    sub sub_{i};
    msm::sm<c> sm{sub_};
    test(static_cast<decltype(sm) &&>(sm));
  }
};

test composite_with_orthogonal_regions = [] {
  struct sub {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
         *idle + event<e4> = s1
        , s1 + event<e4> = s2
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = X
        // ----------------------------------
        ,*idle2 + event<e1> = state<sub>
        , state<sub> + event<e3> = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;

  expect(sm.is(idle, idle2));
  sm.process_event(e1());
  expect(sm.is(s1, msm::state<sub>));

  // expect(subsm.is(idle));
  sm.process_event(e4());
  // expect(subsm.is(s1));
  sm.process_event(e4());
  // expect(subsm.is(s2));

  sm.process_event(e2());
  sm.process_event(e3());
  // expect(subsm.is(s2));
  expect(sm.is(msm::X, msm::X));
};

test composite_with_orthogonal_regions_explicit_entry = [] {
  struct sub {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
         (*idle) + event<e1> = s1
        , s1 + event<e2> = s2
        // ----------------------------------
        , (*idle2) + event<e3> = s3
        , s3 + event<e4> = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
       (*state<sub>) + event<e5> = s1
      , s1 + event<e6> = state<sub>(s2, s3) // explicit entry
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(msm::state<sub>));
  // expect(subsm.is(idle, idle2));

  sm.process_event(e1());
  expect(sm.is(msm::state<sub>));
  // expect(subsm.is(s1, idle2));

  sm.process_event(e5());
  expect(sm.is(s1));
  // expect(subsm.is(s1, idle2));

  sm.process_event(e6());  // go back to sub
  expect(sm.is(msm::state<sub>));
  // expect(subsm.is(s2, s3));
};

test composite_with_orthogonal_regions_explicit_entry_deduce_region = [] {
  struct sub {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
         (*idle) + event<e1> = s1
        , s1 + event<e2> = s2
        // ----------------------------------
        , (*idle2) + event<e3> = s3
        , s3 + event<e4> = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        (*state<sub>) + event<e5> = s1
       , s1 + event<e6> = state<sub>(s3) // explicit entry, deduce second region
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  expect(sm.is(msm::state<sub>));
  // expect(subsm.is(idle, idle2));

  sm.process_event(e1());
  expect(sm.is(msm::state<sub>));
  // expect(subsm.is(s1, idle2));

  sm.process_event(e5());
  expect(sm.is(s1));
  // expect(subsm.is(s1, idle2));

  sm.process_event(e6());  // go back to sub
  expect(sm.is(msm::state<sub>));
  // expect(subsm.is(idle, s3));
};
