//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_STATE_MACHINE_HPP
#define BOOST_SML_BACK_STATE_MACHINE_HPP

#include "boost/sml/aux_/type_traits.hpp"
#include "boost/sml/aux_/utility.hpp"
#include "boost/sml/back/concepts/callable.hpp"
#include "boost/sml/back/concepts/configurable.hpp"
#include "boost/sml/back/concepts/stringable.hpp"
#include "boost/sml/back/concepts/transitional.hpp"
#include "boost/sml/back/internal.hpp"
#include "boost/sml/back/mappings.hpp"
#include "boost/sml/back/policies.hpp"
#include "boost/sml/back/transitions.hpp"

namespace detail {

template <class, class, class TEvent>
struct get_all_events_impl {
  using type = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<>, aux::type_list<TEvent>>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<TSrc, TDst, unexpected_event<TEvent>> {
  using type = aux::type_list<TEvent>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<sm<TSrc>, TDst, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm<TSrc>::events>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<TSrc, sm<TDst>, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm<TDst>::events>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<sm<TSrc>, sm<TDst>, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm<TSrc>::events, typename sm<TDst>::events>;
};
template <class, class TEvent>
struct get_sub_internal_events_impl {
  using type = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<TEvent>, aux::type_list<>>;
};
template <class T, class TEvent>
struct get_sub_internal_events_impl<sm<T>, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm_impl<T>::sub_internal_events>;
};
template <class... Ts>
using get_all_events =
    aux::join_t<typename get_all_events_impl<typename Ts::src_state, typename Ts::dst_state, typename Ts::event>::type...>;
template <class... Ts>
using get_sub_internal_events =
    aux::join_t<typename get_sub_internal_events_impl<typename Ts::src_state, typename Ts::event>::type...>;
template <class... Ts>
using get_events = aux::type_list<typename Ts::event...>;
template <class T>
struct get_exception : aux::type_list<> {};
template <class T>
struct get_exception<exception<T>> : aux::type_list<exception<T>> {};
template <class... Ts>
using get_exceptions = aux::join_t<typename get_exception<Ts>::type...>;
template <class... Ts>
using get_states = aux::join_t<aux::type_list<typename Ts::src_state, typename Ts::dst_state>...>;
template <class... Ts>
using get_initial_states =
    aux::join_t<aux::conditional_t<Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>...>;
template <class... Ts>
using get_history_states =
    aux::join_t<aux::conditional_t<!Ts::history && Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>...>;
template <class>
no_policy get_policy(...);
template <class T, class TPolicy>
TPolicy get_policy(aux::pair<T, TPolicy> *);
template <class SM, class... TPolicies>
struct sm_policy {
  using sm = SM;
  using thread_safety_policy = decltype(get_policy<detail::thread_safety_policy__>((aux::inherit<TPolicies...> *)0));
  using defer_queue_policy = decltype(get_policy<defer_queue_policy__>((aux::inherit<TPolicies...> *)0));
  using logger_policy = decltype(get_policy<logger_policy__>((aux::inherit<TPolicies...> *)0));
  template <class T>
  using rebind = sm_policy<T, TPolicies...>;
};
template <class>
struct get_sub_sm : aux::type_list<> {};
template <class T>
struct get_sub_sm<sm<T>> : aux::join_t<aux::type_list<T>, typename sm<T>::sub_sms> {};
template <class... Ts>
using get_sub_sms = aux::join_t<typename get_sub_sm<Ts>::type...>;
template <class... Ts>
using get_sm_t = aux::type_list<typename Ts::sm...>;
template <class... Ts>
using merge_deps = aux::join_t<typename Ts::deps...>;

template <class TSM>
class sm_impl {
  template <class>
  friend class sm_impl;
  template <class>
  friend class sm;
  template <class>
  friend struct state;
  template <class...>
  friend struct transition;
  template <class...>
  friend struct transitions;
  template <class...>
  friend struct transitions_sub;

 public:
  template <class T>
  using rebind = sm<sm_policy<T>>;
  using sm_t = typename TSM::sm;
  using thread_safety_t = typename TSM::thread_safety_policy::type;
  template <class T>
  using defer_queue_t = typename TSM::defer_queue_policy::template rebind<T>;
  using logger_t = typename TSM::logger_policy::type;
  using has_logger = aux::integral_constant<bool, !aux::is_same<logger_t, no_policy>::value>;
  using transitions_t = decltype(aux::declval<sm_t>()());
  using mappings_t = detail::mappings_t<transitions_t>;
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using states_ids_t = aux::apply_t<aux::type_id, states_t>;
  using initial_states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_initial_states, transitions_t>>;
  using initial_states_ids_t = aux::apply_t<aux::type_id, initial_states_t>;
  using initial_but_not_history_states_t = aux::apply_t<get_history_states, transitions_t>;
  using has_history_states =
      aux::integral_constant<bool, aux::size<initial_states_t>::value != aux::size<initial_but_not_history_states_t>::value>;
  using events_t = aux::apply_t<aux::unique_t, aux::apply_t<get_events, transitions_t>>;
  using sub_internal_events = aux::apply_t<get_sub_internal_events, transitions_t>;
  using events_ids_t = aux::apply_t<aux::pool, aux::apply_t<aux::unique_t, aux::join_t<sub_internal_events, events_t>>>;
  using defer = aux::apply_t<aux::variant, events_t>;
  using defer_t = defer_queue_t<defer>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using state_t = aux::conditional_t<(aux::size<states_t>::value > 0xFF), unsigned short, aux::byte>;
  static constexpr auto regions = aux::size<initial_states_t>::value > 0 ? aux::size<initial_states_t>::value : 1;
  static_assert(regions > 0, "At least one initial state is required");

  template <class... TStates>
  using get_ids = aux::index_sequence<aux::get_id<states_ids_t, -1, TStates>()...>;

 public:
  sm_impl(const aux::init &, const aux::pool_type<sm_t &> *t) : transitions_((t->value)()) {
    initialize(typename sm_impl<TSM>::initial_states_t{});
  }

  sm_impl(const aux::init &, ...) : transitions_(sm_t{}()) { initialize(typename sm_impl<TSM>::initial_states_t{}); }

  sm_impl(sm_impl &&) = default;
  sm_impl(const sm_impl &) = delete;
  sm_impl &operator=(const sm_impl &) = delete;

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
  using exceptions = aux::apply_t<aux::unique_t, aux::apply_t<get_exceptions, events_t>>;
  using has_exceptions = aux::integral_constant<bool, (aux::size<exceptions>::value > 0)>;
#endif  // __pph__

  template <class TEvent, class TDeps, class TSub>
  bool process_event(const TEvent &event, TDeps &deps, TSub &sub) {
    log_process_event<logger_t, sm_t>(has_logger{}, deps, event);

    struct self {
      using type __attribute__((unused)) = sm_impl;
      using state_t __attribute__((unused)) = sm_impl::state_t;
      using has_logger __attribute__((unused)) = sm_impl::has_logger;
      TDeps &deps_;
      sm_impl &me_;
      TSub &sub_sms_;
    } const self_{deps, *this, sub};

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    const auto handled = process_event_noexcept(event, self_, has_exceptions{});
#else   // __pph__
    const auto handled = process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self_, states_t{},
                                                                                     aux::make_index_sequence<regions>{});
#endif  // __pph__
    process_internal_event(self_, anonymous{});
    process_defer_events(self_, handled, aux::type<defer_queue_t<TEvent>>{}, events_t{});

    return handled;
  }

  template <class TDeps, class TSub>
  void start(TDeps &deps, TSub &sub) {
    struct self {
      using type __attribute__((unused)) = sm_impl;
      using state_t __attribute__((unused)) = sm_impl::state_t;
      using has_logger __attribute__((unused)) = sm_impl::has_logger;
      TDeps &deps_;
      sm_impl &me_;
      TSub &sub_sms_;
    } const self_{deps, *this, sub};

    if (!process_internal_event(self_, anonymous{})) {
      process_internal_event(self_, on_entry{});
    }
  }

 private:
  void initialize(const aux::type_list<> &) {}

  template <class... TStates>
  void initialize(const aux::type_list<TStates...> &) {
    auto region = 0, i = region;
    int _[]{0, (region = i, current_state_[region] = aux::get_id<states_ids_t, 0, TStates>(), ++i, 0)...};
    (void)_;
  }

  template <class TSelf, class TEvent, __BOOST_SML_REQUIRES(!aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  bool process_internal_event(TSelf &, const TEvent &, ...) {
    return false;
  }

  template <class TSelf, class TEvent, __BOOST_SML_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  bool process_internal_event(TSelf &self, const TEvent &event) {
    log_process_event<logger_t, sm_t>(has_logger{}, self.deps_, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    return process_event_noexcept(event, self, has_exceptions{});
#else   // __pph__
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{},
                                                                       aux::make_index_sequence<regions>{});
#endif  // __pph__
  }

  template <class TSelf, class TEvent, __BOOST_SML_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  bool process_internal_event(TSelf &self, const TEvent &event, state_t &current_state) {
    log_process_event<logger_t, sm_t>(has_logger{}, self.deps_, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    return process_event_noexcept(event, self, current_state, has_exceptions{});
#else  // __pph__
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{}, current_state);
#endif  // __pph__
  }

  template <class TMappings, class TEvent, class TSelf, class... TStates>
  bool process_event_impl(const TEvent &event, TSelf &self, const aux::type_list<TStates...> &,
                          const aux::index_sequence<0> &) {
    static bool (*dispatch_table[sizeof...(TStates)])(
        TSelf &, const TEvent &, state_t &) = {&get_state_mapping_t<TStates, TMappings>::template execute<TSelf, TEvent>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state_[0]](self, event, current_state_[0]);
  }

  template <class TMappings, class TEvent, class TSelf, class... TStates, int... Ns>
  bool process_event_impl(const TEvent &event, TSelf &self, const aux::type_list<TStates...> &,
                          const aux::index_sequence<Ns...> &) {
    static bool (*dispatch_table[sizeof...(TStates)])(
        TSelf &, const TEvent &, state_t &) = {&get_state_mapping_t<TStates, TMappings>::template execute<TSelf, TEvent>...};
    auto handled = false;
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    int _[]{0, (handled |= dispatch_table[current_state_[Ns]](self, event, current_state_[Ns]), 0)...};
    (void)_;
    return handled;
  }

  template <class TMappings, class TEvent, class TSelf, class... TStates>
  bool process_event_impl(const TEvent &event, TSelf &self, const aux::type_list<TStates...> &, state_t &current_state) {
    static bool (*dispatch_table[sizeof...(TStates)])(
        TSelf &, const TEvent &, state_t &) = {&get_state_mapping_t<TStates, TMappings>::template execute<TSelf, TEvent>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state](self, event, current_state);
  }

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
  template <class TEvent, class TSelf>
  bool process_event_noexcept(const TEvent &event, TSelf &self, const aux::false_type &) noexcept {
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{},
                                                                       aux::make_index_sequence<regions>{});
  }

  template <class TEvent, class TSelf>
  bool process_event_noexcept(const TEvent &event, TSelf &self, state_t &current_state, const aux::false_type &) noexcept {
    return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{}, current_state);
  }

  template <class TEvent, class TSelf>
  bool process_event_noexcept(const TEvent &event, TSelf &self, state_t &current_state, const aux::true_type &) noexcept {
    try {
      return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{}, current_state);
    } catch (...) {
      return process_exception(self, exceptions{});
    }
  }

  template <class TEvent, class TSelf>
  bool process_event_noexcept(const TEvent &event, TSelf &self, const aux::true_type &) {
    try {
      return process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{},
                                                                         aux::make_index_sequence<regions>{});
    } catch (...) {
      return process_exception(self, exceptions{});
    }
  }

  template <class TSelf>
  bool process_exception(TSelf &self, const aux::type_list<> &) {
    return process_internal_event(self, exception<_>{});
  }

  template <class TSelf, class E, class... Es>
  bool process_exception(TSelf &self, const aux::type_list<E, Es...> &) {
    try {
      throw;
    } catch (const typename E::type &e) {
      return process_internal_event(self, E{e});
    } catch (...) {
      return process_exception(self, aux::type_list<Es...>{});
    }
  }
#endif  // __pph__

  template <class TSelf, class... TEvents>
  void process_defer_events(TSelf &, const bool, const aux::type<detail::no_policy> &, const aux::type_list<TEvents...> &) {}

  template <class TSelf, class TEvent>
  bool process_event_no_deffer(TSelf &self, const void *data) {
    const auto &event = *static_cast<const TEvent *>(data);
    log_process_event<logger_t, sm_t>(has_logger{}, self.deps_, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    const auto handled = process_event_noexcept(event, self, has_exceptions{});
#else   // __pph__
    const auto handled = process_event_impl<get_event_mapping_t<TEvent, mappings_t>>(event, self, states_t{},
                                                                                     aux::make_index_sequence<regions>{});
#endif  // __pph__
    if (handled) {
      event.~TEvent();
      defer_.pop();
    }
    return handled;
  }

  template <class TSelf, class T, class... TEvents>
  void process_defer_events(TSelf &self, const bool handled, const aux::type<T> &, const aux::type_list<TEvents...> &) {
    if (handled) {
      auto size = defer_.size();
      static bool (sm_impl::*dispatch_event[sizeof...(TEvents)])(
          TSelf &, const void *) = {&sm_impl::process_event_no_deffer<TSelf, TEvents>...};
      while (size-- && (this->*dispatch_event[defer_.front().id])(self, defer_.front().data))
        ;
    }
  }

  template <class TVisitor, class... TStates>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<0> &) const {
    static void (*dispatch_table[sizeof...(TStates)])(const TVisitor &) = {&sm_impl::visit_state<TVisitor, TStates>...};
    dispatch_table[current_state_[0]](visitor);
  }

  template <class TVisitor, class... TStates, int... Ns>
  void visit_current_states_impl(const TVisitor &visitor, const aux::type_list<TStates...> &,
                                 const aux::index_sequence<Ns...> &) const {
    static void (*dispatch_table[sizeof...(TStates)])(const TVisitor &) = {&sm_impl::visit_state<TVisitor, TStates>...};
    int _[]{0, (dispatch_table[current_state_[Ns]](visitor), 0)...};
    (void)_;
  }

  template <class TVisitor, class TState>
  static void visit_state(const TVisitor &visitor) {
    visitor(state<TState>{});
  }

  template <class, class TSelf, class TSrcState, class TDstState>
  void update_current_state(TSelf &, state_t &, const state_t &, const TSrcState &, const TDstState &, const aux::true_type &) {
  }

  template <class TExplicit, class TSelf, class TSrcState, class TDstState>
  void update_current_state(TSelf &self, state_t &current_state, const state_t &new_state, const TSrcState &src_state,
                            const TDstState &dst_state, const aux::false_type &) {
    update_current_state_impl<TExplicit>(self, current_state, new_state, src_state, dst_state);
  }

  template <class, class TSelf, class TSrcState, class TDstState>
  void update_current_state_impl(TSelf &self, state_t &current_state, const state_t &new_state, const TSrcState &src_state,
                                 const TDstState &dst_state) {
    process_internal_event(self, on_exit{}, current_state);
    log_state_change<logger_t, sm_t>(has_logger{}, self.deps_, src_state, dst_state);
    current_state = new_state;
    process_internal_event(self, on_entry{}, current_state);
  }

  template <class TExplicit, class TSelf, class TSrcState, class T>
  void update_current_state_impl(TSelf &self, state_t &current_state, const state_t &new_state, const TSrcState &src_state,
                                 const state<sm<T>> &dst_state) {
    process_internal_event(self, on_exit{}, current_state);
    log_state_change<logger_t, sm_t>(has_logger{}, self.deps_, src_state, dst_state);
    current_state = new_state;
    update_composite_states<sm_impl<T>>(self, TExplicit{}, typename sm_impl<T>::has_history_states{},
                                        typename sm_impl<T>::initial_but_not_history_states_t{});
    process_internal_event(self, on_entry{}, current_state);
  }

  template <class T, class TSelf, class... Ts>  // explicit
  void update_composite_states(TSelf &self, const aux::type_list<Ts...> &, ...) {
    auto &sm = aux::try_get<T>(&self.sub_sms_);
    int _[]{0, (sm.current_state_[sm.template get_region<Ts>()] = aux::get_id<typename T::states_ids_t, -1, Ts>(), 0)...};
    (void)_;
    // TODO
  }

  template <class T, class TSelf, class... Ts,
            class... THs>  // history states, no explicit
  void
  update_composite_states(TSelf &self, const aux::type_list<> &, const aux::true_type &, const aux::type_list<THs...> &) {
    auto &sm = aux::try_get<T>(&self.sub_sms_);
    int _[]{0, (sm.current_state_[aux::get_id<typename T::initial_states_ids_t, -1, THs>()] =
                    aux::get_id<typename T::states_ids_t, -1, THs>(),
                0)...};
    (void)_;
  }

  template <class T, class TSelf>  // just initials, no explicit
  void update_composite_states(TSelf &self, const aux::type_list<> &, const aux::false_type &, ...) {
    aux::try_get<T>(&self.sub_sms_).initialize(typename T::initial_states_t{});
  }

  template <class TState>
  static constexpr auto get_region() noexcept {
    return get_region_impl(aux::get_id<states_ids_t, -1, TState>(), aux::apply_t<get_ids, initial_states_t>{});
  }

  template <int... Ids>
  static constexpr auto get_region_impl(int id, const aux::index_sequence<Ids...> &) noexcept {
    auto region = 0, i = 0;
    int _[]{0, (id < Ids ? region : region = i, ++i)...};
    (void)_;
    return region;
  }

  auto create_lock(const aux::type<detail::no_policy> &) { return detail::no_policy{}; }

  template <class T>
  auto create_lock(const aux::type<T> &) {
    struct lock_guard {
      explicit lock_guard(thread_safety_t &synch) : thread_safety_(synch) { thread_safety_.lock(); }
      ~lock_guard() { thread_safety_.unlock(); }
      thread_safety_t &thread_safety_;
    };
    return lock_guard{thread_safety_};
  }

  transitions_t transitions_;

 public:
  state_t current_state_[regions];

  thread_safety_t thread_safety_;
  defer_t defer_;
};

template <class TSM>
class sm {
 public:
  using type = TSM;
  template <class T>
  using rebind = sm<sm_policy<T>>;
  using sm_t = typename TSM::sm;
  template <class T>
  using defer_queue_t = typename TSM::defer_queue_policy::template rebind<T>;
  using logger_t = typename TSM::logger_policy::type;
  using transitions_t = decltype(aux::declval<sm_t>()());
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  template <class>
  struct convert;

  template <class... Ts>
  struct convert<aux::type_list<Ts...>> {
    using type = aux::type_list<sm_impl<Ts>...>;
  };
  using sub_sms = aux::apply_t<get_sub_sms, states_t>;
  using sm_all_t = aux::apply_t<aux::inherit, aux::join_t<aux::type_list<sm_t>, aux::apply_t<get_sm_t, sub_sms>>>;
  using sub_sms_t =
      aux::apply_t<aux::pool, typename convert<aux::join_t<aux::type_list<TSM>, aux::apply_t<get_sub_sms, states_t>>>::type>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using deps_t = aux::apply_t<
      aux::pool, aux::apply_t<aux::unique_t, aux::join_t<deps, aux::type_list<logger_t>, aux::apply_t<merge_deps, sub_sms_t>>>>;

  template <class... TDeps>
  using dependable = aux::is_same<aux::bool_list<aux::always<TDeps>::value...>,
                                  aux::bool_list<aux::is_base_of<aux::remove_reference_t<TDeps>, sm_all_t>::value ||
                                                 aux::is_base_of<aux::pool_type<TDeps>, deps_t>::value...>>;

 public:
  using states = typename sm_impl<TSM>::states_t;
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_all_events, transitions_t>>;
  using transitions = aux::apply_t<aux::type_list, transitions_t>;

 private:
  using events_ids_t = aux::apply_t<aux::pool, events>;

 public:
  sm(sm &&) = default;
  sm(const sm &) = delete;
  sm &operator=(const sm &) = delete;

  template <class... TDeps, __BOOST_SML_REQUIRES(dependable<TDeps...>::value)>
  explicit sm(TDeps &&... deps) : deps_{aux::init{}, aux::pool<TDeps...>{deps...}}, sub_sms_{aux::pool<TDeps...>{deps...}} {
    static_cast<aux::pool_type<sm_impl<TSM>> &>(sub_sms_).value.start(deps_, sub_sms_);
  }

  explicit sm(deps_t &deps) : deps_(deps), sub_sms_{deps} { start(aux::type<sub_sms_t>{}); }

  template <class TEvent, __BOOST_SML_REQUIRES(aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  void process_event(const TEvent &event) {
    static_cast<aux::pool_type<sm_impl<TSM>> &>(sub_sms_).value.process_event(event, deps_, sub_sms_);
  }

  template <class TEvent, __BOOST_SML_REQUIRES(!aux::is_base_of<aux::pool_type<TEvent>, events_ids_t>::value)>
  void process_event(const TEvent &) {
    static_cast<aux::pool_type<sm_impl<TSM>> &>(sub_sms_).value.process_event(unexpected_event<>{}, deps_, sub_sms_);
  }

  template <class TEvent>
  void process_event(const event<TEvent> &) {
    process_event(TEvent{});
  }

  template <class T = sm_t, class TVisitor, __BOOST_SML_REQUIRES(concepts::callable<void, TVisitor>::value)>
  void visit_current_states(const TVisitor &visitor) const {
    using sm = sm_impl<typename TSM::template rebind<T>>;
    using states_t = typename sm::states_t;
    constexpr auto regions = sm::regions;
    static_cast<const aux::pool_type<sm> &>(sub_sms_).value.visit_current_states_impl(visitor, states_t{},
                                                                                      aux::make_index_sequence<regions>{});
  }

  template <class T = sm_t, class TState>
  bool is(const state<TState> &) const {
    auto result = false;
    visit_current_states<T>([&](auto state) { result |= aux::is_same<TState, typename decltype(state)::type>::value; });
    return result;
  }

  template <class T = sm_t, class... TStates,
            __BOOST_SML_REQUIRES(sizeof...(TStates) == sm_impl<typename TSM::template rebind<T>>::regions)>
  bool is(const state<TStates> &...) const {
    auto result = true;
    auto i = 0;
    using states_ids_t = typename sm_impl<TSM>::states_ids_t;
    int state_ids[] = {aux::get_id<states_ids_t, 0, TStates>()...};
    visit_current_states<T>(
        [&](auto state) { result &= (aux::get_id<states_ids_t, 0, typename decltype(state)::type>() == state_ids[i++]); });
    return result;
  }

 protected:
  deps_t deps_;
  sub_sms_t sub_sms_;
};

}  // detail

#endif
