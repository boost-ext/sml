//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_POLICIES_HPP
#define BOOST_SML_BACK_POLICIES_HPP

#include "boost/sml/aux_/utility.hpp"
#include "boost/sml/back/policies/defer_queue.hpp"
#include "boost/sml/back/policies/dispatch.hpp"
#include "boost/sml/back/policies/logger.hpp"
#include "boost/sml/back/policies/process_queue.hpp"
#include "boost/sml/back/policies/testing.hpp"
#include "boost/sml/back/policies/thread_safety.hpp"
#include "boost/sml/back/utility.hpp"  // rebind_impl

namespace back {

struct no_policy : policies::thread_safety_policy__ {
  using type = no_policy;
  template <class>
  using rebind = no_policy;
  template <class...>
  using defer = no_policy;
  using const_iterator = no_policy;
  using flag = no_policy;
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};

template <class TDefault, class>
TDefault get_policy(...);

template <class, class T, class TPolicy>
TPolicy get_policy(aux::pair<T, TPolicy> *);

template <class SM, class... TPolicies>
struct sm_policy {
  static_assert(aux::is_same<aux::remove_reference_t<SM>, SM>::value, "SM type can't have qualifiers");

  using sm = SM;
  using thread_safety_policy =
      decltype(get_policy<no_policy, policies::thread_safety_policy__>((aux::inherit<TPolicies...> *)0));
  using defer_queue_policy = decltype(get_policy<no_policy, policies::defer_queue_policy__>((aux::inherit<TPolicies...> *)0));
  using process_queue_policy =
      decltype(get_policy<no_policy, policies::process_queue_policy__>((aux::inherit<TPolicies...> *)0));
  using logger_policy = decltype(get_policy<no_policy, policies::logger_policy__>((aux::inherit<TPolicies...> *)0));
  using testing_policy = decltype(get_policy<no_policy, policies::testing_policy__>((aux::inherit<TPolicies...> *)0));
  using default_dispatch_policy = policies::jump_table;
  using dispatch_policy =
      decltype(get_policy<default_dispatch_policy, policies::dispatch_policy__>((aux::inherit<TPolicies...> *)0));

  template <class T>
  using rebind = typename rebind_impl<T, TPolicies...>::type;
};

}  // namespace back

#endif
