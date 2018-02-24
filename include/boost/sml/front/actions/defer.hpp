//
// Copyright (c) 2016-2018 Kris Jusiak (kris at jusiak dot net)
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

  template <class T>
  static void move_impl(aux::byte (&data)[size], defer_event &&other) {
    new (&data) T(static_cast<T &&>(*reinterpret_cast<T *>(other.data)));
  }

 public:
  defer_event(defer_event &&other) : id(other.id), dtor(other.dtor), move(other.move) {
    move(data, static_cast<defer_event &&>(other));
  }

  defer_event(const defer_event &) = delete;
  defer_event &operator=(const defer_event &) = delete;

  template <class T>
  defer_event(T object) {  // non explicit
    id = aux::get_id<int, T>((ids_t *)0);
    dtor = &dtor_impl<T>;
    move = &move_impl<T>;
    new (&data) T(static_cast<T &&>(object));
  }

  ~defer_event() { dtor(data); }

  alignas(alignment) aux::byte data[size];
  int id = -1;

 private:
  void (*dtor)(aux::byte *);
  void (*move)(aux::byte (&)[size], defer_event &&);
};

struct defer : action_base {
  template <class TEvent, class TSM, class TDeps, class TSubs>
  void operator()(const TEvent &event, TSM &sm, TDeps &, TSubs &) {
    sm.defer_.push(event);
  }
};
}  // actions
}  // front::actions

#endif
