//
// Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#if (__cplusplus >= 201703L && __has_include(<variant>))
#include <boost/sml.hpp>
#include <cassert>
#include <iostream>
#include <variant>

namespace sml = boost::sml;

namespace {
struct connect {};
struct disconnect {};

class data {
  struct Disconnected {};
  struct Connected {
    int id{};
  };

 public:
  explicit data(const std::string& address) : address{address} {}

  inline auto operator()() {
    using namespace boost::sml;
    // clang-format off
    return make_transition_table(
      * state<Disconnected> + event<connect> / [this] {
          data_ = Connected{42};
        } = state<Connected>
      , state<Connected> + event<disconnect> / [this] {
          std::cout << std::get<Connected>(data_).id << std::endl;
        } = X
    );
    // clang-format on
  }

 private:
  std::string address{};
  std::variant<Disconnected, Connected> data_{};
};
}  // namespace

int main() {
  sml::sm<data> sm{data{"127.0.0.1"}};
  sm.process_event(connect{});
  sm.process_event(disconnect{});
  assert(sm.is(sml::X));
}
#else
int main() {}
#endif
