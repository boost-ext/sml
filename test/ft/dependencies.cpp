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
