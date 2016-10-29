//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
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
    void operator()(const T&, TSM& sm, TDeps& deps, TSubs& subs) {
      sm.process_event(event, deps, subs);
    }

   private:
    TEvent event;
  };

  template <class TEvent>
  auto operator()(const TEvent& event) {
    return process_impl<TEvent>{event};
  }
};
}
}  // front::actions

#endif
