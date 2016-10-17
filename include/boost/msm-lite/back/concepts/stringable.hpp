#ifndef STRINGABLE_61TPC07G
#define STRINGABLE_61TPC07G

#include "boost/msm-lite/aux_/type_traits.hpp"

namespace concepts {
template <class T, class = decltype(T::c_str())>
aux::true_type test_stringable(const T &);
aux::false_type test_stringable(...);
template <class T, class = void>
struct stringable : aux::false_type {};
template <class T>
struct stringable<T, decltype(void(sizeof(T)))> : decltype(test_stringable(aux::declval<T>())) {};
}  // concepts

#endif
