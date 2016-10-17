#ifndef CALLABLE_IF8N0N6C
#define CALLABLE_IF8N0N6C

#include "boost/msm-lite/aux_/type_traits.hpp"

namespace concepts {
struct callable_fallback {
  void operator()();
};
template <class T>
aux::false_type test_callable(aux::non_type<void (callable_fallback::*)(), &T::operator()> *);
template <class>
aux::true_type test_callable(...);
template <class, class>
struct callable_impl : aux::false_type {};
template <class R>
struct callable_impl<R, aux::true_type> : aux::true_type {};
template <class R, class T>
using callable = callable_impl<R, decltype(test_callable<aux::inherit<T, callable_fallback>>(0))>;

}  // concepts

#endif
