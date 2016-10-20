//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_INTERNAL_HPP
#define BOOST_SML_BACK_INTERNAL_HPP

namespace detail {

struct internal_event {
  static auto c_str() { return "internal_event"; }
};
struct anonymous : internal_event {
  static auto c_str() { return "anonymous"; }
};
struct on_entry : internal_event {
  static auto c_str() { return "on_entry"; }
};
struct on_exit : internal_event {
  static auto c_str() { return "on_exit"; }
};

}  // detail
