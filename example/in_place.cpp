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

int main() {
#if defined(__cpp_deduction_guides)
  namespace sml = boost::sml;

  struct start {};

  sml::front::sm sm = [] {
    using namespace sml;
    return make_transition_table(*"idle"_s + event<start> / [] { std::cout << "action\n"; } = X);
  };

  sm.process_event(start{});
  assert(sm.is(sml::X));
#endif
}
