#ifndef CONFIGURABLE_UPXBJTZ8
#define CONFIGURABLE_UPXBJTZ8

#include "boost/msm-lite/aux_/type_traits.hpp"
#include "boost/msm-lite/aux_/utility.hpp"

namespace concepts {
template <class T>
decltype(aux::declval<T>()()) configurable_impl(int);
template <class>
void configurable_impl(...);
template <class T>
struct configurable : aux::is_pool<decltype(configurable_impl<T>(0))> {};
}  // concepts

#endif
