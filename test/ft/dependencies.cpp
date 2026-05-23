//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_SML_CREATE_DEFAULT_CONSTRUCTIBLE_DEPS
#include <boost/sml.hpp>
#include <array>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};
struct e6 {};

const auto idle = sml::state<class idle>;

test minimal_with_dependency = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      return make_transition_table(*idle + event<e1> / [](int i) { expect(42 == i); });
    }
  };

  sml::sm<c> sm{42};
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(idle));
};

#if !defined(_MSC_VER)
test default_dependency = [] {
  struct data {
    int id{};
  };

  struct c {
    auto operator()() {
      using namespace sml;
      return make_transition_table(*idle + event<e1>[([](data& d) { return d.id == 42; })] = X);
    }
  };

  {
    sml::sm<c> sm{};
    expect(sm.is(idle));
    sm.process_event(e1{});
    expect(sm.is(idle));
  }

  {
    sml::sm<c> sm{data{42}};
    expect(sm.is(idle));
    sm.process_event(e1{});
    expect(sm.is(sml::X));
  }

  {
    data d{42};
    sml::sm<c> sm{d};
    expect(sm.is(idle));
    sm.process_event(e1{});
    expect(sm.is(sml::X));
  }
};
#endif

test dependencies = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;

      auto guard = [](int i) {
        expect(42 == i);
        return true;
      };

      auto action = [](double d, e1) { expect(d == 87.0); };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [ guard ] / (action, [](const auto& e, int i) -> void {
            (void)e;
            expect(std::is_same<decltype(e), const e1&>::value);
            expect(42 == i);
          }) = X
      );
      // clang-format on
    }
  };

  {
    sml::sm<c> sm{42, 87.0};
    sm.process_event(e1{});
    expect(sm.is(sml::X));
  }

  {
    sml::sm<c> sm{87.0, 42};
    sm.process_event(e1{});
    expect(sm.is(sml::X));
  }
};

test dependencies_with_const = [] {
  struct dep {
    int i = 7;
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;

      auto guard = [](int i, dep& dependency) {
        expect(7 == dependency.i);
        expect(42 == i);
        return true;
      };

      auto const_guard = [](dep const& dependency, int const i) {
        expect(7 == dependency.i);
        expect(42 == i);
        return true;
      };

      auto action = [](int i, dep& dependency, e1) {
        expect(7 == dependency.i);
        expect(42 == i);
      };

      auto const_action = [](dep const& dependency, int const i, e1) {
        expect(7 == dependency.i);
        expect(42 == i);
      };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [ guard && const_guard ] / (action, const_action) = X
      );
      // clang-format on
    }
  };

  {
    dep dependency;
    sml::sm<c> sm{dependency, 42};
    sm.process_event(e1{});
    expect(sm.is(sml::X));
  }
};

test dependencies_smart_ptrs = [] {
  struct Data {
    bool m_member{true};
  };

  struct c {
    auto operator()() noexcept {
      const auto guard = [](std::shared_ptr<Data> data) { return data->m_member; };
      const auto action = [](const std::shared_ptr<Data>& data) { expect(data->m_member); };

      using namespace sml;
      return make_transition_table(*idle + event<e1>[guard] / action = X);
    }
  };

  auto data = std::make_shared<Data>();
  sml::sm<c> sm{data};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

test dependencies_with_reference = [] {
  struct Data {
    int m_member{42};
  };

  struct c {
    auto operator()() noexcept {
      const auto action = [](Data& data) { expect(data.m_member == 42); };

      using namespace sml;
      return make_transition_table(*idle + event<e1> / action = X);
    }
  };

  Data data;
  sml::sm<c> sm{data};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

test dependencies_with_const_reference = [] {
  struct Data {
    int m_member{42};
  };

  struct c {
    auto operator()() noexcept {
      const auto action = [](const Data& data) { expect(data.m_member == 42); };

      using namespace sml;
      return make_transition_table(*idle + event<e1> / action = X);
    }
  };

  Data data;
  sml::sm<c> sm{data};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

test dependencies_with_pointer = [] {
  struct Data {
    int m_member{42};
  };

  struct c {
    auto operator()() noexcept {
      const auto action = [](Data* data) { expect(data->m_member == 42); };

      using namespace sml;
      return make_transition_table(*idle + event<e1> / action = X);
    }
  };

  Data data;
  sml::sm<c> sm{&data};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

test dependencies_with_const_pointer = [] {
  struct Data {
    int m_member{42};
  };

  struct c {
    auto operator()() noexcept {
      const auto action = [](const Data* data) { expect(data->m_member == 42); };

      using namespace sml;
      return make_transition_table(*idle + event<e1> / action = X);
    }
  };

  Data data;
  sml::sm<c> sm{&data};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

test dependencies_move_only_reference = [] {
  struct Data {
    std::unique_ptr<int> m_member;
  };

  struct c {
    auto operator()() noexcept {
      const auto action = [](const Data& data) {
        expect(data.m_member != nullptr);
        expect(*data.m_member == 42);
      };

      using namespace sml;
      return make_transition_table(*idle + event<e1> / action = X);
    }
  };

  Data data;
  data.m_member = std::make_unique<int>(42);
  sml::sm<c> sm{data};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

#if (_MSC_VER >= 1910)  // MSVC 2017
test dependencies_multiple_subs = [] {
  struct update {
    int id = 0;
  };

  struct data {
    int i = 0;
  };

  struct dep {
    std::string name;
  };

  struct action1 {
    void operator()(dep& common) {
      expect("text" == common.name);
      expect(42 == d.i);
      ++d.i;
    }

    data& d;
  };

  struct sub {
    data d;

    auto operator()() {
      const auto action2 = [](data& d) {
        return [&d](dep& common) {
          expect("text" == common.name);
          expect(43 == d.i);
          --d.i;
        };
      };

      using namespace sml;
      return make_transition_table(*idle + event<e1> / (action1{d}, action2(d)));
    }
  };

  struct top {
    auto operator()() const {
      using namespace sml;
      return make_transition_table(*idle +
                                       event<update> / [](const auto& event, std::array<boost::sml::sm<sub>, 5>& subs) -> void {
        subs[event.id].process_event(e1{});
      });
    }
  };

  dep d{"text"};
  data da{42};

  std::array<boost::sml::sm<sub>, 5> subs{{boost::sml::sm<sub>{d, sub{da}}, boost::sml::sm<sub>{d, sub{da}},
                                           boost::sml::sm<sub>{d, sub{da}}, boost::sml::sm<sub>{d, sub{da}},
                                           boost::sml::sm<sub>{d, sub{da}}}};

  sml::sm<top> sm{subs};
  sm.process_event(update{0});
  sm.process_event(update{1});
  sm.process_event(update{2});

  for (auto i = 0u; i < subs.size(); ++i) {
    sub& s = subs[i];
    expect(42 == s.d.i);
  }
};
#endif

// Issue #504: pool_type_impl<T&>'s (init, object) constructor used a comma
// expression ': value(i, object)' to "initialize" a reference member.
// (i, object) is the C++ comma operator — it evaluates i, discards the result,
// then evaluates object, whose value becomes the initializer.  For a T& member
// this bound the reference to the 'object' function parameter, which is a
// dangling reference after the constructor returns.
// Fix: initialize the backing store value_ via try_get, then bind value to value_.
// This test exercises the pool(const pool<TArgs...>&) copy-constructor path that
// instantiates pool_type_impl<T&>(const init&, const TObject&).
struct dep504 {
  int val = 99;
};

test ref_dep_copy_from_pool_not_dangling = [] {
  // SM with a reference dep (dep504&) and a sub-SM so that the
  // pool(const pool<TArgs...>&) path is exercised during sm construction.
  struct sub504 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](dep504& d) { expect(99 == d.val); };
      // clang-format off
      return make_transition_table(*idle + event<e2> / check = X);
      // clang-format on
    }
  };

  struct top504 {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(*idle + event<e1> = sml::state<sub504>);
      // clang-format on
    }
  };

  dep504 dep;
  sml::sm<top504> sm{dep};
  sm.process_event(e1{});
  // e2 triggers the check action inside sub504 which calls expect(99 == d.val),
  // verifying the dep reference is valid (not dangling).
  sm.process_event(e2{});
};

// Issue #485: passing a pointer dependency as an lvalue caused the SM to store
// nullptr instead of the actual pointer.  Root cause: forwarding reference
// deduction wraps a T* lvalue as T*&; the pool init constructor's try_get
// lookup had no overload for pool_type<T*&>*, so it fell through to the
// missing_ctor_parameter catch-all which returned nullptr.
// Fix: add try_get overloads for reference-to-pointer types (T*& and const T*&).
struct dep485 {
  int val = 42;
};

test pointer_dep_lvalue_not_null = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // action takes non-const pointer: Dep*
      auto action = [](dep485* d) { expect(d != nullptr); expect(42 == d->val); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / action = X);
      // clang-format on
    }
  };

  dep485 dep;
  dep485* ptr = &dep;  // lvalue pointer — was getting stored as nullptr before the fix
  sml::sm<c> sm{ptr};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

test const_pointer_dep_lvalue_not_null = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // action takes const pointer: const Dep*
      auto action = [](const dep485* d) { expect(d != nullptr); expect(42 == d->val); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / action = X);
      // clang-format on
    }
  };

  dep485 dep;
  dep485* ptr = &dep;  // non-const Dep* lvalue: should be implicitly convertible to const Dep*
  sml::sm<c> sm{ptr};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};
