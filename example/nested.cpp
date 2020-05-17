//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cassert>

#if defined(__GNUC__) && !defined(__clang__)
int main() {}
#else

namespace sml = boost::sml;

namespace {
template <class = class Dummy>  // Use a dummy template to delay POI and support nested SM
class top {
  struct e1 {};

  struct nested {
    auto operator()() const noexcept {
      using namespace sml;
      return make_transition_table(*"idle"_s + event<e1> = X);
    }
  };

 public:
  void process() {
    sm.process_event(e1{});
    assert(sm.is(sml::X));
  }

 private:
  sml::sm<nested> sm{};
};
}  // namespace

int main() {
  top<> sm{};
  sm.process();
}
#endif
