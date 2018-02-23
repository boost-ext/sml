//
// Copyright (c) 2016-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_POLICIES_THREAD_SAFETY_HPP
#define BOOST_SML_BACK_POLICIES_THREAD_SAFETY_HPP

#include "boost/sml/aux_/utility.hpp"

namespace back {
namespace policies {

struct thread_safety_policy__ {};

template <class T>
struct thread_safe : aux::pair<thread_safety_policy__, thread_safe<T>> {
  using type = T;
};

}  // policies
}  // back

#endif
