//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <string>
#include <utility>

namespace sml = boost::sml;

#if !defined(_MSC_VER)
test events = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *"idle"_s + "e1"_e = "s1"_s
        , "s1"_s   + "e2"_e = "s2"_s
        , "s2"_s   + "e3"_e / [](const auto& event) {
            auto e = event;
            static_assert(aux::is_same<aux::string<char, 'e', '3'>, decltype(e)>::value, "");
          } = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  using namespace sml;
  expect(sm.is("idle"_s));
  sm.process_event("e1"_e());
  expect(sm.is("s1"_s));
  sm.process_event("e2"_e());
  expect(sm.is("s2"_s));
  sm.process_event("e3"_e());
  expect(sm.is(X));
};
#endif
