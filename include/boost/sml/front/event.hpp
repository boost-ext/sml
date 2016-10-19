#ifndef EVENT_94X8WBIE
#define EVENT_94X8WBIE

#include "boost/sml/front/fwd.hpp"

namespace detail {

template <class>
struct event {
  template <class T, BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const {
    return transition_eg<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }

  template <class T, BOOST_SML_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const {
    return transition_ea<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }
};

}  // detail

#endif
