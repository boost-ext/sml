//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <string>
#include <utility>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};

/*const auto idle = sml::state<class idle>;*/
/*const auto s1 = sml::state<class s1>;*/

//test process_event = [] {
  //struct c {
    //auto operator()() {
      //using namespace sml;
      //// clang-format off
      //return make_transition_table(
         //*idle + event<e1> = s1
        //, s1 + event<e2> / process(e3{})
        //, s1 + event<e3> / [this] { a_called++; } = X
      //);
      //// clang-format on
    //}

    //int a_called = 0;
  //};

  //c c_;
  //sml::sm<c> sm{c_};
  //expect(sm.is(idle));
  //sm.process_event(e1{});
  //expect(sm.is(s1));
  //expect(!c_.a_called);
  //sm.process_event(e2{});  // + process(e3{})
  //expect(1 == c_.a_called);
  //expect(sm.is(sml::X));
//};
