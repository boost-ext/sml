//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_FRONT_ACTIONS_PROCESS_HPP
#define BOOST_SML_FRONT_ACTIONS_PROCESS_HPP

#include "boost/sml/front/operators.hpp"

namespace front {
namespace actions {

struct process {
  template <class TEvent>
  class process_impl : public action_base {
   public:
    explicit process_impl(const TEvent& event) : event(event) {}

    template <class T, class TSM, class TDeps, class TSubs>
    void operator()(const T&, TSM&, TDeps&, TSubs& subs) {
      aux::get<get_root_sm_t<TSubs>>(subs).process_.push(event);
    }

   private:
    TEvent event;
  };

  template <class TEvent>
  auto operator()(const TEvent& event) {
    return process_impl<TEvent>{event};
  }
};

}  // namespace actions
}  // namespace front

#endif
