//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_FRONT_ACTIONS_DEFER_HPP
#define BOOST_SML_FRONT_ACTIONS_DEFER_HPP

#include "boost/sml/front/operators.hpp"

namespace front {
namespace actions {

template <class... Ts>
class defer_event {
  using ids_t = aux::type_id<Ts...>;
  static constexpr auto alignment = aux::max<alignof(Ts)...>();
  static constexpr auto size = aux::max<sizeof(Ts)...>();

  template <class T>
  static void dtor_impl(aux::byte *data) {
    (void)data;
    reinterpret_cast<T *>(data)->~T();
  }

 public:
  defer_event(defer_event && other):defer_event(other) {
    other.id = -1;
  }

  defer_event &operator=(const defer_event &) = delete;

  template <class T>
  defer_event(T object) {  // non explicit
    id = aux::get_id<int, T>((ids_t *)0);
    dtor = &dtor_impl<T>;
    new (&data) T(static_cast<T &&>(object));
  }

  ~defer_event() { if (id != -1) dtor(data); }

  alignas(alignment) aux::byte data[size];
  int id = -1;

 private:
  defer_event(const defer_event &) = default;
  void (*dtor)(aux::byte *);
};

struct defer : action_base {
  template <class TEvent, class TSM, class TDeps, class TSubs>
  void operator()(const TEvent &event, TSM &sm, TDeps &, TSubs &) {
    sm.defer_.push(event);
  }
};
}
}  // front::actions

#endif
