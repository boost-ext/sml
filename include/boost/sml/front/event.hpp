//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_FRONT_EVENT_HPP
#define BOOST_SML_FRONT_EVENT_HPP

#include "boost/sml/aux_/utility.hpp"
#include "boost/sml/concepts/callable.hpp"

namespace front {

template <class, class>
struct transition_eg;

template <class, class>
struct transition_ea;

template <class TEvent>
struct event {
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const {
    return transition_eg<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }

  template <class T, __BOOST_SML_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const {
    return transition_ea<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }

  auto operator()() const { return TEvent{}; }
};

}  // namespace front

#endif
