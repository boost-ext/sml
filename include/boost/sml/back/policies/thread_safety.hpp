//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_POLICIES_THREAD_SAFETY_HPP
#define BOOST_SML_BACK_POLICIES_THREAD_SAFETY_HPP

#include "boost/sml/aux_/utility.hpp"

namespace back {
namespace policies {

struct thread_safety_policy__ {
  auto create_lock() { return *this; }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};

template <class TLock>
struct thread_safe : aux::pair<thread_safety_policy__, thread_safe<TLock>> {
  using type = thread_safe;

  auto create_lock() {
    struct lock_guard {
      explicit lock_guard(TLock &lock) : lock_{lock} { lock_.lock(); }
      ~lock_guard() { lock_.unlock(); }
      TLock &lock_;
    };
    return lock_guard{lock};
  }

  TLock lock;
};

}  // namespace policies
}  // namespace back

#endif
