//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_FRONT_ACTIONS_DEFER_HPP
#define BOOST_SML_FRONT_ACTIONS_DEFER_HPP

#include "boost/sml/front/fwd.hpp"

namespace detail {

struct defer : action_base {
  template <class TEvent, class TSM, class TDeps>
  void operator()(const TEvent& event, TSM& sm, TDeps&) {
    sm.defer_.push(event);
  }
};

}  // detail
#endif
