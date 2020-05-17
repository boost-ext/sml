//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_POLICIES_PROCESS_QUEUE_HPP
#define BOOST_SML_BACK_POLICIES_PROCESS_QUEUE_HPP

#include "boost/sml/aux_/utility.hpp"

namespace back {
namespace policies {

struct process_queue_policy__ {};

template <template <class...> class T>
struct process_queue : aux::pair<back::policies::process_queue_policy__, process_queue<T>> {
  template <class U>
  using rebind = T<U>;
};

}  // namespace policies
}  // namespace back

#endif
