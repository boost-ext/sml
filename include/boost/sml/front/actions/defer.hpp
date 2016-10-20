//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DEFER_W9M9BM8H
#define DEFER_W9M9BM8H

#include "boost/sml/front/fwd.hpp"

namespace detail {

struct defer : action_base {
  template <class TSelf, class TEvent>
  void operator()(TSelf& self, const TEvent& event) {
    self.defer_.push(event);
  }
};

}  // detail
#endif
