//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_INTERNALS_HPP
#define BOOST_SML_BACK_INTERNALS_HPP

#include "boost/sml/back/queue_handler.hpp"

namespace back {

struct _ {};
struct initial {};
struct unexpected {};
struct entry_exit {};

struct terminate_state {
  static auto c_str() { return "terminate"; }
};

struct internal_event {
  static auto c_str() { return "internal_event"; }
};

struct anonymous : internal_event {
  static auto c_str() { return "anonymous"; }
};

template <class T, class TEvent = T>
struct on_entry : internal_event, entry_exit {
  static auto c_str() { return "on_entry"; }
  explicit on_entry(const TEvent& event = {}) : event_(event) {}
  const TEvent& event_;
};

template <class T, class TEvent = T>
struct on_exit : internal_event, entry_exit {
  static auto c_str() { return "on_exit"; }
  explicit on_exit(const TEvent& event = {}) : event_(event) {}
  const TEvent& event_;
};

template <class T, class TException = T>
struct exception : internal_event {
  using type = TException;
  explicit exception(const TException& exception = {}) : exception_(exception) {}
  const TException& exception_;
};

template <class T, class TEvent = T>
struct unexpected_event : internal_event, unexpected {
  explicit unexpected_event(const TEvent& event = {}) : event_(event) {}
  const TEvent& event_;
};

template <class TEvent>
struct event_type {
  using event_t = TEvent;
  using generic_t = TEvent;
  using mapped_t = void;
};

template <class TEvent>
struct event_type<exception<TEvent>> {
  using event_t = TEvent;
  using generic_t = exception<TEvent>;
  using mapped_t = void;
};

template <class TEvent, class T>
struct event_type<unexpected_event<T, TEvent>> {
  using event_t = TEvent;
  using generic_t = unexpected_event<T>;
  using mapped_t = void;
};

template <class TEvent, class T>
struct event_type<on_entry<T, TEvent>> {
  using event_t = TEvent;
  using generic_t = on_entry<T>;
  using mapped_t = on_entry<T, TEvent>;
};

template <class TEvent, class T>
struct event_type<on_exit<T, TEvent>> {
  using event_t = TEvent;
  using generic_t = on_exit<T>;
  using mapped_t = on_exit<T, TEvent>;
};

template <class TEvent>
using get_event_t = typename event_type<TEvent>::event_t;

template <class TEvent>
using get_generic_t = typename event_type<TEvent>::generic_t;

template <class TEvent>
using get_mapped_t = typename event_type<TEvent>::mapped_t;

template <class... TEvents>
struct process : queue_handler<TEvents...> {
  using queue_handler<TEvents...>::queue_handler;
};

template <class... TEvents>
struct defer : deque_handler<TEvents...> {
  using deque_handler<TEvents...>::deque_handler;
};

}  // namespace back

#endif
