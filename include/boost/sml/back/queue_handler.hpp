//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_QUEUE_HANDLER_HPP
#define BOOST_SML_BACK_QUEUE_HANDLER_HPP

#include "boost/sml/aux_/type_traits.hpp"
#include "boost/sml/aux_/utility.hpp"

namespace back {

template <class... Ts>
class queue_event {
  using ids_t = aux::type_id<Ts...>;
  static constexpr auto alignment = aux::max_element<alignof(Ts)...>();
  static constexpr auto size = aux::max_element<sizeof(Ts)...>();

  template <class T>
  static void dtor_impl(aux::byte *data) {
    (void)data;
    reinterpret_cast<T *>(data)->~T();
  }

  template <class T>
  static void move_impl(aux::byte (&data)[size], queue_event &&other) {
    new (&data) T(static_cast<T &&>(*reinterpret_cast<T *>(other.data)));
  }

 public:
  queue_event(queue_event &&other) : id(other.id), dtor(other.dtor), move(other.move) {
    move(data, static_cast<queue_event &&>(other));
  }
  queue_event &operator=(queue_event &&other) {
    dtor(data);

    id = other.id;
    dtor = other.dtor;
    move = other.move;
    move(data, static_cast<queue_event &&>(other));
    return *this;
  }

  queue_event(const queue_event &) = delete;
  queue_event &operator=(const queue_event &) = delete;

  template <class T>
  queue_event(T object) {  // non explicit
    id = aux::get_id<int, T>((ids_t *)0);
    dtor = &dtor_impl<T>;
    move = &move_impl<T>;
    new (&data) T(static_cast<T &&>(object));
  }

  ~queue_event() { dtor(data); }

  alignas(alignment) aux::byte data[size];
  int id = -1;

 private:
  void (*dtor)(aux::byte *);
  void (*move)(aux::byte (&)[size], queue_event &&);
};

template <class TEvent>
class queue_event_call {
  using call_t = void (*)(void *, const TEvent &);

 public:
  queue_event_call() = default;
  explicit queue_event_call(const call_t &call) : call{call} {}
  call_t call{};
};

template <class... TEvents>
struct queue_handler : queue_event_call<TEvents>... {
  queue_handler() = default;

  template <class TQueue, class = typename TQueue::container_type>
  explicit queue_handler(TQueue &queue)
      : queue_event_call<TEvents>(queue_handler::push_impl<TQueue, TEvents>)..., queue_{&queue} {}

  template <class TEvent>
  void operator()(const TEvent &event) {
    static_cast<queue_event_call<TEvent> *>(this)->call(queue_, event);
  }

 private:
  template <class TQueue, class TEvent>
  static auto push_impl(void *queue, const TEvent &event) {
    static_cast<TQueue *>(queue)->push(event);
  }

  void *queue_{};
};

template <class... TEvents>
struct deque_handler : queue_event_call<TEvents>... {
  deque_handler() = default;

  template <class TDeque, class = typename TDeque::allocator_type>
  explicit deque_handler(TDeque &deque)
      : queue_event_call<TEvents>(deque_handler::push_impl<TDeque, TEvents>)..., deque_{&deque} {}

  template <class TEvent>
  void operator()(const TEvent &event) {
    static_cast<queue_event_call<TEvent> *>(this)->call(deque_, event);
  }

 private:
  template <class TDeque, class TEvent>
  static auto push_impl(void *deque, const TEvent &event) {
    static_cast<TDeque *>(deque)->push_back(event);
  }

  void *deque_{};
};

}  // namespace back

#endif
