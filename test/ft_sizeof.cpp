//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm-lite.hpp"

namespace msm = boost::msm::lite;

test transition_sizeof = [] {
  using namespace msm;
  constexpr auto i = 0;

  {
  auto t = "state"_s + "event"_t [([]{})];
  static_expect(0 == sizeof(t));
  }

  {
  auto t = "state"_s + "event"_t / []{};
  static_expect(0 == sizeof(t));
  }

  {
  auto t = "state"_s + "event"_t [([]{})] / []{};
  static_expect(0 == sizeof(t));
  }

  {
  auto t = "state"_s + "event"_t [([](int){})] / []{};
  static_expect(0 == sizeof(t));
  }

  {
  auto t = "state"_s + "event"_t [([]{})] / [](int){};
  static_expect(0 == sizeof(t));
  }

  {
  auto t = "state"_s + "event"_t [([](int){})] / [](int){};
  static_expect(0 == sizeof(t));
  }

  {
  auto t = "state"_s + "event"_t [([](int, float){})] / [](double, const int&){};
  static_expect(0 == sizeof(t));
  }

  {
  auto t = "state"_s + "event"_t [([i]{})] / []{};
  static_expect(sizeof(i) == sizeof(t));
  }

  {
  auto t = "state"_s + "event"_t [([]{})] / [i]{};
  static_expect(sizeof(i) == sizeof(t));
  }

  {
  auto t = "state"_s + "event"_t [([]{})] / [&i]{};
  static_expect(sizeof(&i) == sizeof(t));
  }
};

test sm_sizeof = [] {
  constexpr auto sm_size = 3; // deps=1 + transitions=1 + current_state=1

  struct empty {
    auto configure() noexcept {
      using namespace msm;
      return make_transition_table();
    }
  };

  static_expect(sm_size == sizeof(msm::sm<empty>{}));

  struct no_capture_transition {
    auto configure() noexcept {
      using namespace msm;
      return make_transition_table(
        *"idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{},
         "idle"_s + "event"_t [([]{})] / []{}
      );
    }
  };
  static_expect(sm_size - 1 /*transitions=0*/ == sizeof(msm::sm<no_capture_transition>));
};
