//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// clang-format off
#if !defined(_MSC_VER)
#if __has_include(<boost/di.hpp>)
// clang-format on
#include <boost/di.hpp>
#include <boost/sml.hpp>

namespace di = boost::di;
namespace sml = boost::sml;

struct e1 {};
struct e2 {};

const auto idle = sml::state<class idle>;
const auto s1 = sml::state<class s1>;

test di_minimal = [] {
  struct c {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *idle + event<e1> / [](int i) { expect(42 == i); }
      );
      // clang-format on
    }
  };

  auto injector = di::make_injector(di::bind<>.to(42));
  auto sm = injector.create<sml::sm<c>>();
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(idle));
};

test di_ctor = [] {
  struct c {
    int &i_;

    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *idle + event<e1> / [this](int i) { expect(i_ == i); }
      );
      // clang-format on
    }
  };

  auto i = 42;
  auto injector = di::make_injector(di::bind<>.to(i));
  auto sm = injector.create<sml::sm<c>>();
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(idle));
};

test di_complex = [] {
  struct i1 {
    virtual ~i1() noexcept = default;
    virtual void dummy() = 0;
  };
  struct impl1 : i1 {
    void dummy() override{};
  };

  struct i2 {
    virtual ~i2() noexcept = default;
    virtual void dummy() = 0;
  };
  struct impl2 : i2 {
    void dummy() override{};
  };

  struct i3 {
    virtual ~i3() noexcept = default;
    virtual void dummy() = 0;
  };
  struct impl3 : i3 {
    void dummy() override{};
  };

  struct i4 {
    virtual ~i4() noexcept = default;
    virtual void dummy() = 0;
  };
  struct impl4 : i4 {
    void dummy() override{};
  };

  struct i5 {
    virtual ~i5() noexcept = default;
    virtual void dummy() = 0;
  };
  struct impl5 : i5 {
    void dummy() override{};
  };

  struct c {
    auto operator()() const {
      using namespace sml;

      auto guard1 = [](int i, const auto &, double d) {
        expect(42 == i);
        expect(87.0 == d);
        return true;
      };

      auto guard2 = [](int i, i1 &p1, i2 &p2, i3 &p3) {
        expect(42 == i);
        expect(dynamic_cast<const impl1 *>(&p1));
        expect(dynamic_cast<const impl2 *>(&p2));
        expect(dynamic_cast<const impl3 *>(&p3));
        return true;
      };

      auto action1 = [](const auto &, double d, int i) {
        expect(42 == i);
        expect(87.0 == d);
      };

      auto action2 = [](const i1 &p1, const i2 &p2, const i3 &p3, const i4 &p4, const i5 &p5) {
        expect(dynamic_cast<const impl1 *>(&p1));
        expect(dynamic_cast<const impl2 *>(&p2));
        expect(dynamic_cast<const impl3 *>(&p3));
        expect(dynamic_cast<const impl4 *>(&p4));
        expect(dynamic_cast<const impl5 *>(&p5));
      };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> / [](int i) { expect(42 == i); } = s1
        , s1 + event<e2> [ guard1 && guard2 ] / (action1, action2) = X
      );
      // clang-format on
    }
  };

  // clang-format off
  auto f = 12.0f;
  const auto injector = di::make_injector(
      di::bind<>.to(42)
    , di::bind<float>.to(f)
    , di::bind<>.to(87.0)
    , di::bind<i1>.to<impl1>()
    , di::bind<i2>.to<impl2>()
    , di::bind<i3>.to<impl3>()
    , di::bind<i4>.to<impl4>()
    , di::bind<i5>.to<impl5>()
  );
  // clang-format on
  auto sm = injector.create<sml::sm<c>>();
  expect(sm.is(idle));

  sm.process_event(e1{});
  expect(sm.is(s1));

  sm.process_event(e2{});
  expect(sm.is(sml::X));
};
#endif
#endif
