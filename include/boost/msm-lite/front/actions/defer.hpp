#ifndef DEFER_W9M9BM8H
#define DEFER_W9M9BM8H

#include "boost/msm-lite/front/fwd.hpp"

namespace detail {

struct defer : action_base {
  template <class TSelf, class TEvent>
  void operator()(TSelf& self, const TEvent& event) {
    self.defer_.push(event);
  }
};

}  // detail
#endif
