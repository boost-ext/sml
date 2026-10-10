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

// Issue #467: passing a derived (non-copyable) type as a base-class reference
// dependency failed to compile after v1.1.3.  The pool init copy-constructor
// called try_get<Base>() which found nothing when the pool held Derived&, fell
// back to missing_ctor_parameter<Base>, tried to default-construct Base, and
// then could not bind the temporary to Base&.
// Fix: a covariant try_get that finds the unique D& / const D& dep with D derived
// from T and returns T& via implicit base-class conversion (see also #715 below).
struct base467 {
  base467() = default;
  base467(const base467 &) = delete;  // non-copyable, like NiceMock<T>
  int val = 7;
};

struct derived467 : base467 {};

test non_copyable_derived_dep_as_base_ref = [] {
  struct c467 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](base467 &d) { expect(7 == d.val); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  derived467 dep;
  // Action takes base467& but we pass derived467 — must compile and work.
  sml::sm<c467> sm{dep};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// Issue #715: the #467 lookup deduced D from the pool's pool_type<D&> bases.
// That deduction fails as soon as the pool holds more than one reference dep,
// and MSVC needs exponential time to find out.  The slot is now found by
// scanning the pool's dep list, so other reference deps must not interfere.
test derived_dep_as_base_ref_among_other_ref_deps = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](base467 &d, int &i) { expect(7 == d.val && 42 == i); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  derived467 dep;
  int i = 42;
  std::string unused;
  sml::sm<c715> sm{unused, dep, i};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

test const_derived_dep_as_const_base_ref = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](const base467 &d) { expect(7 == d.val); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  const derived467 dep;
  int i = 42;
  sml::sm<c715> sm{dep, i};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// A const derived dep next to another const reference dep: before #715 the const
// covariant lookup needed the pool's only const reference dep.  iface715 is
// abstract, so a missed lookup cannot fall back to a default-constructed object.
struct iface715 {
  virtual ~iface715() = default;
  virtual int id() const = 0;
};
struct impl715a : iface715 {
  int id() const override { return 1; }
};
struct impl715b : iface715 {
  int id() const override { return 2; }
};

test const_derived_dep_as_const_base_ref_among_other_const_ref_deps = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](const iface715 &f, const int &i) { expect(2 == f.id() && 42 == i); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  const impl715b impl{};
  const int i = 42;
  sml::sm<c715> sm{impl, i};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// With a mutable and a const derived dep, a const lookup reads the single const one,
// as #9 did before #715.  base467& needs the covariant lookup among other reference deps.
test const_derived_dep_not_hidden_by_mutable_derived_dep = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](base467 &b, const iface715 &f) { expect(7 == b.val && 2 == f.id()); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  derived467 derived;
  impl715a a;
  const impl715b b{};
  sml::sm<c715> sm{derived, a, b};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// A mutable lookup cannot bind a const derived dep, so next to one it reads the single
// mutable derived dep.
test mutable_derived_dep_not_hidden_by_const_derived_dep = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](iface715 &f) { expect(1 == f.id()); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  impl715a a;
  const impl715b b{};
  sml::sm<c715> sm{a, b};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// An exact base467& dep must win over a derived467& dep that would also match covariantly.
// Without the exact-dep check, base467& itself would be a second candidate (a hard error), or,
// were it excluded, the covariant overload (taking the pool itself) would win.
test exact_base_dep_preferred_over_derived_dep = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](base467 &d) { expect(1 == d.val); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  base467 base;
  base.val = 1;
  derived467 derived;
  sml::sm<c715> sm{derived, base};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// Before #715 the const covariant overload was more specialized than the exact
// T& one, so a const derived467 dep won over an exact base467& dep, and the
// base467& slot could not be initialized.
test exact_base_dep_preferred_over_const_derived_dep = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](base467 &d) { expect(1 == d.val); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  base467 base;
  base.val = 1;
  const derived467 derived{};
  sml::sm<c715> sm{base, derived};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// A base held by value is no exact dep: a derived dep wins over it, as before #715.
// try_get returns a copy of the base, which the const base& slot would bind as a
// dangling temporary.
struct copyable_base715 {
  int val = 1;
};
struct copyable_derived715a : copyable_base715 {
  copyable_derived715a() { val = 715; }
};
struct copyable_derived715b : copyable_base715 {
  copyable_derived715b() { val = 716; }
};

test derived_dep_preferred_over_base_held_by_value = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](const copyable_base715 &b) { expect(715 == b.val); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  copyable_derived715a derived;
  copyable_base715 base;
  base.val = 5;
  sml::sm<c715> sm{std::move(base), derived};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// Also next to another reference dep and for a base taken by value.  Before #715 the
// deduction of D failed there, so the base held by value was read.
test derived_dep_preferred_over_base_held_by_value_among_other_ref_deps = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](copyable_base715 b, int &i) { expect(715 == b.val && 42 == i); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  copyable_derived715a derived;
  copyable_base715 base;
  base.val = 5;
  int i = 42;
  sml::sm<c715> sm{std::move(base), derived, i};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// A dep with the base as a private base cannot be read as the base.  With a base held
// by value that one is read, as before #715 next to another reference dep, instead of
// the error that replaces a silent fallback otherwise (errors/private_base_dep.cpp);
// not for a const base& slot, which would bind a dangling copy
// (errors/private_base_dep_next_to_base_by_value.cpp).
struct private_derived715 : private copyable_base715 {};

test base_held_by_value_read_next_to_private_derived_dep = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](copyable_base715 b, int &i) { expect(5 == b.val && 42 == i); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  private_derived715 derived;
  copyable_base715 base;
  base.val = 5;
  int i = 42;
  sml::sm<c715> sm{std::move(base), derived, i};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// A volatile derived dep is no candidate for a base that is not volatile: neither a
// base& nor a const base& can bind it.  So the base held by value is read, as before.
test volatile_derived_dep_is_no_candidate = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](copyable_base715 b, int &i) { expect(5 == b.val && 42 == i); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  volatile copyable_derived715a derived;
  copyable_base715 base;
  base.val = 5;
  int i = 42;
  sml::sm<c715> sm{std::move(base), derived, i};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// BOOST_SML_CREATE_DEFAULT_CONSTRUCTIBLE_DEPS (defined above) lets a base& slot hold a
// copy, also a sliced copy of a const derived dep.  Next to a mutable derived dep the
// slot binds that one itself, as without the macro.
test mutable_derived_dep_bound_not_copied_next_to_const_derived_dep = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](copyable_base715 &b) {
        expect(715 == b.val);
        b.val = 42;
      };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  copyable_derived715a a;
  const copyable_derived715b b{};
  sml::sm<c715> sm{a, b};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
  expect(42 == a.val);
};

// With several derived deps the base held by value is read, as before #715, when
// the deduction of D failed, instead of the error for several derived deps.
test base_held_by_value_read_next_to_several_derived_deps = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](copyable_base715 b) { expect(5 == b.val); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  copyable_derived715a a;
  copyable_derived715b b;
  copyable_base715 base;
  base.val = 5;
  sml::sm<c715> sm{std::move(base), a, b};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// void is not a class: its lookup must neither form void& nor take every reference
// dep as derived from void (any D* converts to void*).  A passed void* dep is read,
// also next to a covariant lookup; an unpassed one stays the null default.
test void_ptr_deps_next_to_derived_dep = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](base467 &b, void *p, int *q) { expect(7 == b.val && p == q && 42 == *q); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };
  struct c715_unpassed {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](int *q, void *p) { expect(42 == *q && nullptr == p); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  derived467 derived;
  int i = 42;
  void *p = &i;
  int *q = &i;
  sml::sm<c715> sm{derived, p, q};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
  sml::sm<c715_unpassed> sm_unpassed{q};
  sm_unpassed.process_event(e1{});
  expect(sm_unpassed.is(sml::X));
};

// The covariant lookup must not be affected by declarations that ADL finds in a
// dep's namespace, e.g. function templates named like helpers that revisions of the
// #715 lookup called unqualified (try_get_slot would be an exact match).
namespace user715 {
struct base {
  int val = 1;
};
struct derived : base {
  derived() { val = 715; }
};
template <class T>
void implicitly_convert_to(T);
template <class T, class TPool>
int try_get_slot(const TPool *, int);
}  // namespace user715

test covariant_dep_lookup_ignores_adl = [] {
  struct c715 {
    auto operator()() noexcept {
      using namespace sml;
      auto check = [](user715::base &b, int &i) { expect(715 == b.val && 42 == i); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / check = X);
      // clang-format on
    }
  };

  user715::derived derived;
  int i = 42;
  sml::sm<c715> sm{derived, i};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// Reference deps need not be complete types, also when a forward-declared type is
// the only reference dep: the lookup of the state machine class itself must not
// apply is_base_of to it (before #715: an error with GCC and MSVC).  The second
// sm also needs the covariant lookup among other reference deps.
struct incomplete715;
struct sm_with_single_incomplete_ref_dep {
  auto operator()() noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(*idle + event<e1> / [](incomplete715 &, int i) { expect(5 == i); } = X);
    // clang-format on
  }
};
struct sm_with_derived_and_incomplete_ref_deps {
  auto operator()() noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(*idle + event<e1> / [](incomplete715 &, base467 &b) { expect(7 == b.val); } = X);
    // clang-format on
  }
};

// Never called: there is no incomplete715 object, only constructing the sms has to compile.
void construct_sm_with_single_incomplete_ref_dep(incomplete715 &dep) {
  sml::sm<sm_with_single_incomplete_ref_dep> sm{dep, 5};
  sm.process_event(e1{});
}
void construct_sm_with_derived_and_incomplete_ref_deps(incomplete715 &dep, derived467 &derived) {
  sml::sm<sm_with_derived_and_incomplete_ref_deps> sm{dep, derived};
  sm.process_event(e1{});
}

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

// Issue #483: dependencies (and SM classes themselves) declared `final` failed
// to compile because `is_empty_base<T>` tried to inherit from T, and `sm_impl`
// tried to subclass the SM class.
// Fix (is_empty): add a partial specialisation guarded by __is_final(T) that
//   reports false without ever instantiating is_empty_base<T>.
// Fix (sm_impl): extend should_not_subclass_statemachine_class to also return
//   true for final SM classes, routing them through the composition path.

struct final_dep483 final {
  int val = 99;
};

test final_dependency_type_compiles = [] {
  // A dependency type declared `final` must be usable as an SM dependency.
  struct c483dep {
    auto operator()() noexcept {
      using namespace sml;
      auto action = [](final_dep483& d) { expect(99 == d.val); };
      // clang-format off
      return make_transition_table(*idle + event<e1> / action = X);
      // clang-format on
    }
  };

  final_dep483 dep;
  sml::sm<c483dep> sm{dep};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

test final_sm_class_compiles = [] {
  // An SM class declared `final` must work via the composition path.
  struct final_sm483 final {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(*idle + event<e2> = X);
      // clang-format on
    }
  };

  sml::sm<final_sm483> sm;
  sm.process_event(e2{});
  expect(sm.is(sml::X));
};

// Issue #530: a guard taking `const State &` received a default-constructed copy
// instead of the live state modified by earlier actions.  Root cause: two separate
// pool slots were created for `State &` (mutable actions) and `const State &`
// (const guards), so mutations through the mutable slot were invisible to the const
// slot.
// Fix: normalise `const T &` → `T &` in ignore::non_events (single pool slot) and
// add a get_arg overload that strips const at lookup time so both see the same object.
#if defined(BOOST_SML_CREATE_DEFAULT_CONSTRUCTIBLE_DEPS)
struct State530 {
  int id{};
};
struct connect530 { int id{}; };
struct interrupt530 {};

test const_ref_state_dep_sees_live_state = [] {
  struct c530 {
    auto operator()() noexcept {
      using namespace sml;
      const auto set    = [](const connect530& ev, State530& s) { s.id = ev.id; };
      // Guard takes const State& — must see the value set by previous `set` action.
      const auto check  = [](const connect530& ev, const State530& s) {
        expect(s.id == 42);   // set by the prior `set` action
        expect(ev.id == 99);
        return true;
      };
      // clang-format off
      return make_transition_table(
        *idle + event<connect530>   / set             = sml::state<State530>,
         sml::state<State530> + event<connect530>[check]           = X
      );
      // clang-format on
    }
  };

  sml::sm<c530> sm;
  sm.process_event(connect530{42});   // -> State530, id = 42
  sm.process_event(connect530{99});   // guard checks const State530& — must see id == 42
  expect(sm.is(sml::X));
};
#endif
