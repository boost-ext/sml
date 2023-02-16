//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cassert>
#include <iostream>

namespace sml = boost::sml;

namespace {
struct printf_logger
{
  template <class SM, class TEvent>
  void log_process_event(const TEvent&)
  {
    std::printf("[%s][event] %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TEvent>());
  }

  template <class SM, class TGuard, class TEvent>
  void log_guard(const TGuard&, const TEvent&, bool result)
  {
    std::printf("[%s][guard] %s(%s): %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TGuard>(),
                sml::aux::get_type_name<TEvent>(), (result ? "accept" : "reject"));
  }

  template <class SM, class TAction, class TEvent>
  void log_action(const TAction&, const TEvent&)
  {
    std::printf("[%s][action] %s(%s)\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TAction>(),
                sml::aux::get_type_name<TEvent>());
  }

  template <class SM, class TSrcState, class TDstState>
  void log_state_change(const TSrcState& src, const TDstState& dst)
  {
    std::printf("[%s][transition] %s -> %s\n", sml::aux::get_type_name<SM>(), src.c_str(), dst.c_str());
  }
};

struct connect {
  int id{};
};
struct disconnect {};
struct interrupt {};

struct Disconnected {};
struct Connected {
  int id{};  /// per state data
};
struct Interrupted {
  int id{};  /// per state data
};

class data {
  using Self = data;

 public:
  explicit data(const std::string& address) : address{address} {}

  auto operator()() {
    using namespace boost::sml;

    // clang-format off
    return make_transition_table(
      * state<Disconnected> + event<connect>    = state<Connected>
      , state<Connected>    + event<interrupt>  = state<Interrupted>
      , state<Interrupted>  + event<connect>    = state<Connected>
      , state<Connected>    + event<disconnect> = X
    );
    // clang-format on
  }

 private:
  void print(Connected& state) { std::cout << address << ':' << state.id << '\n'; };

  std::string address{};  /// shared data between states
};

}  // namespace

printf_logger logger;

int main() {
  data d{std::string{"127.0.0.1"}};
  sml::sm<data, sml::logger<printf_logger>> sm{d, Connected{1}, logger};
  sm.process_event(connect{1024});
  sm.process_event(interrupt{});
  sm.process_event(connect{1025});
  sm.process_event(disconnect{});
  assert(sm.is(sml::X));
}
