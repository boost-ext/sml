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
#include "boost/sml/back/internals.hpp"
#include "boost/sml/back/mappings.hpp"
#include "boost/sml/back/utility.hpp"
#include "boost/sml/concepts/callable.hpp"

namespace back {

template <class TSM>
struct sm_impl {
  using sm_t = typename TSM::sm;
  using thread_safety_t = typename TSM::thread_safety_policy::type;
  using defer_policy_t = typename TSM::defer_queue_policy;
  template <class T>
  using defer_queue_t = typename defer_policy_t::template rebind<T>;
  template <class... Ts>
  using defer_event_t = typename defer_policy_t::template defer<Ts...>;
  using logger_t = typename TSM::logger_policy::type;
  using transitions_t = decltype(aux::declval<sm_t>().operator()());
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using states_ids_t = aux::apply_t<aux::type_id, states_t>;
  using initial_states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_initial_states, transitions_t>>;
  using initial_states_ids_t = aux::apply_t<aux::type_id, initial_states_t>;
  using history_states_t = aux::apply_t<get_history_states, transitions_t>;
  using has_history_states =
      aux::integral_constant<bool, aux::size<initial_states_t>::value != aux::size<history_states_t>::value>;
  using sub_internal_events_t = aux::apply_t<get_sub_internal_events, transitions_t>;
  using events_t = aux::apply_t<aux::unique_t, aux::join_t<sub_internal_events_t, aux::apply_t<get_events, transitions_t>>>;
  using events_ids_t = aux::apply_t<aux::inherit, events_t>;
  using has_unexpected_events = typename aux::is_base_of<unexpected, aux::apply_t<aux::inherit, events_t>>::type;
  using has_entry_exits = typename aux::is_base_of<entry_exit, aux::apply_t<aux::inherit, events_t>>::type;
  using defer_t = defer_queue_t<aux::apply_t<defer_event_t, events_t>>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using state_t = aux::conditional_t<(aux::size<states_t>::value > 0xFF), unsigned short, aux::byte>;
  static constexpr auto regions = aux::size<initial_states_t>::value;
  static_assert(regions > 0, "At least one initial state is required");
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
  using exceptions = aux::apply_t<aux::unique_t, aux::apply_t<get_exceptions, events_t>>;
  using has_exceptions = aux::integral_constant<bool, (aux::size<exceptions>::value > 0)>;
#endif  // __pph__
  struct mappings : mappings_t<transitions_t> {};

  sm_impl(const aux::init &, const aux::pool_type<sm_t &> *t) : transitions_((t->value)()) {
    initialize(typename sm_impl<TSM>::initial_states_t{});
  }
  sm_impl(const aux::init &, ...) : transitions_(sm_t{}()) { initialize(typename sm_impl<TSM>::initial_states_t{}); }
  sm_impl(sm_impl &&) = default;
  sm_impl(const sm_impl &) = delete;
  sm_impl &operator=(const sm_impl &) = delete;

  template <class TEvent, class TDeps, class TSubs>
  bool process_event(const TEvent &event, TDeps &deps, TSubs &subs) {
    log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    const auto handled =
        process_event_noexcept<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#else   // __pph__
    const auto handled = process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(
        event, deps, subs, states_t{}, aux::make_index_sequence<regions>{});
#endif  // __pph__
    process_internal_events(anonymous{}, deps, subs);
    process_defer_events(deps, subs, handled, aux::type<defer_queue_t<TEvent>>{}, events_t{});

    return handled;
  }

  void initialize(const aux::type_list<> &) {}

  template <class... TStates>
  void initialize(const aux::type_list<TStates...> &) {
    auto region = 0, i = region;
    (void)aux::swallow{0, (region = i, current_state_[region] = aux::get_id<state_t, TStates>((states_ids_t *)0), ++i, 0)...};
  }

  template <class TDeps, class TSubs>
  void start(TDeps &deps, TSubs &subs) {
    process_internal_events(on_entry<_, initial>{}, deps, subs);
    process_internal_events(anonymous{}, deps, subs);
  }

  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(!aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &, TDeps &, TSubs &, ...) {
    return false;
  }

  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &event, TDeps &deps, TSubs &subs) {
    log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    return process_event_noexcept<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#else   // __pph__
    return process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                    aux::make_index_sequence<regions>{});
#endif  // __pph__
  }

  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &event, TDeps &deps, TSubs &subs) {
    log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    return process_event_noexcept<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#else   // __pph__
    return process_event_impl<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                   aux::make_index_sequence<regions>{});
#endif  // __pph__
  }

  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(!aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_event(const TEvent &, TDeps &, TSubs &, ...) {
    return false;
  }

  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_event(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state) {
    log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    return process_event_noexcept<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, current_state,
                                                                                        has_exceptions{});
#else   // __pph__
    return process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                    current_state);
#endif  // __pph__
  }

  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_event(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state) {
    log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    return process_event_noexcept<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, current_state,
                                                                                       has_exceptions{});
#else   // __pph__
    return process_event_impl<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                   current_state);
#endif  // __pph__
  }

  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &,
                          aux::index_sequence<0>) {
    using dispatch_table_t = bool (*)(const TEvent &, sm_impl &, TDeps &, TSubs &, state_t &);
    const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &get_state_mapping_t<TStates, TMappings, has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state_[0]](event, *this, deps, subs, current_state_[0]);
  }

  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates, int... Ns>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &,
                          aux::index_sequence<Ns...>) {
    using dispatch_table_t = bool (*)(const TEvent &, sm_impl &, TDeps &, TSubs &, state_t &);
    const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &get_state_mapping_t<TStates, TMappings, has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>...};
    auto handled = false;
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    (void)aux::swallow{0, (handled |= dispatch_table[current_state_[Ns]](event, *this, deps, subs, current_state_[Ns]), 0)...};
    return handled;
  }

  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &,
                          state_t &current_state) {
    using dispatch_table_t = bool (*)(const TEvent &, sm_impl &, TDeps &, TSubs &, state_t &);
    const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &get_state_mapping_t<TStates, TMappings, has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state](event, *this, deps, subs, current_state);
  }

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, aux::false_type) noexcept {
    return process_event_impl<TMappings>(event, deps, subs, states_t{}, aux::make_index_sequence<regions>{});
  }

  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state, aux::false_type) noexcept {
    return process_event_impl<TMappings>(event, deps, subs, states_t{}, current_state);
  }

  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state, aux::true_type) noexcept {
    try {
      return process_event_impl<TMappings>(event, deps, subs, states_t{}, current_state);
    } catch (...) {
      return process_exception(deps, subs, exceptions{});
    }
  }

  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, aux::true_type) {
    try {
      return process_event_impl<TMappings>(event, deps, subs, states_t{}, aux::make_index_sequence<regions>{});
    } catch (...) {
      return process_exception(deps, subs, exceptions{});
    }
  }

  template <class TDeps, class TSubs>
  bool process_exception(TDeps &deps, TSubs &subs, const aux::type_list<> &) {
    return process_internal_events(exception<_>{}, deps, subs);
  }

  template <class TDeps, class TSubs, class E, class... Es>
  bool process_exception(TDeps &deps, TSubs &subs, const aux::type_list<E, Es...> &) {
    try {
      throw;
    } catch (const typename E::type &e) {
      return process_internal_events(E{e}, deps, subs);
    } catch (...) {
      return process_exception(deps, subs, aux::type_list<Es...>{});
    }
  }
#endif  // __pph__

  template <class TDeps, class TSubs, class... TEvents>
  void process_defer_events(TDeps &, TSubs &, const bool, const aux::type<no_policy> &, const aux::type_list<TEvents...> &) {}

  template <class TDeps, class TSubs, class TEvent>
  bool process_event_no_deffer(TDeps &deps, TSubs &subs, const void *data) {
    const auto &event = *static_cast<const TEvent *>(data);
    log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)  // __pph__
    const auto handled = process_event_noexcept<get_event_mapping_t<TEvent, mappings>>(event, deps, subs, has_exceptions{});
#else   // __pph__
    const auto handled = process_event_impl<get_event_mapping_t<TEvent, mappings>>(event, deps, subs, states_t{},
                                                                                   aux::make_index_sequence<regions>{});
#endif  // __pph__
    if (handled) {
      defer_.pop();
    }
    return handled;
  }

  template <class TDeps, class TSubs, class TDeferQueue, class... TEvents>
  void process_defer_events(TDeps &deps, TSubs &subs, const bool handled, const aux::type<TDeferQueue> &,
                            const aux::type_list<TEvents...> &) {
    if (handled) {
      auto size = defer_.size();
      using dispatch_table_t = bool (sm_impl::*)(TDeps &, TSubs &, const void *);
      const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TEvents))] = {
          &sm_impl::process_event_no_deffer<TDeps, TSubs, TEvents>...};
      while (size-- && (this->*dispatch_table[defer_.front().id])(deps, subs, defer_.front().data))
        ;
    }
  }

  template <class TVisitor, class... TStates>
  void visit_current_states(const TVisitor &visitor, const aux::type_list<TStates...> &, aux::index_sequence<0>) const {
    using dispatch_table_t = void (*)(const TVisitor &);
    const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &sm_impl::visit_state<TVisitor, TStates>...};
    dispatch_table[current_state_[0]](visitor);
  }

  template <class TVisitor, class... TStates, int... Ns>
  void visit_current_states(const TVisitor &visitor, const aux::type_list<TStates...> &, aux::index_sequence<Ns...>) const {
    using dispatch_table_t = void (*)(const TVisitor &);
    const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &sm_impl::visit_state<TVisitor, TStates>...};
    (void)aux::swallow{0, (dispatch_table[current_state_[Ns]](visitor), 0)...};
  }

  template <class TVisitor, class TState>
  static void visit_state(const TVisitor &visitor) {
    visitor(aux::string<TState>{});
  }

  no_policy create_lock(const aux::type<no_policy> &) { return {}; }

  template <class TLockPolicy>
  auto create_lock(const aux::type<TLockPolicy> &) {
    struct lock_guard {
      explicit lock_guard(thread_safety_t &synch) : thread_safety_(synch) { thread_safety_.lock(); }
      ~lock_guard() { thread_safety_.unlock(); }
      thread_safety_t &thread_safety_;
    };
    return lock_guard{thread_safety_};
  }

  bool is_terminated() const { return is_terminated_impl(aux::make_index_sequence<regions>{}); }
  bool is_terminated_impl(aux::index_sequence<0>) const {
    return current_state_[0] == aux::get_id<state_t, terminate_state>((states_ids_t *)0);
  }
  template <int... Ns>
  bool is_terminated_impl(aux::index_sequence<Ns...>) const {
    auto result = true;
    (void)aux::swallow{
        0, (current_state_[Ns] == aux::get_id<state_t, terminate_state>((states_ids_t *)0) ? result : result = false)...};
    return result;
  }

  transitions_t transitions_;
  state_t current_state_[regions];
  thread_safety_t thread_safety_;
  defer_t defer_;
};

template <class TSM>
class sm {
  using sm_t = typename TSM::sm;
  using logger_t = typename TSM::logger_policy::type;
  using logger_dep_t =
      aux::conditional_t<aux::is_same<no_policy, logger_t>::value, aux::type_list<>, aux::type_list<logger_t &>>;
  using transitions_t = decltype(aux::declval<sm_t>().operator()());

 public:
  using states = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using state_machines = aux::apply_t<get_sub_sms, states>;
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_all_events, transitions_t>>;
  using transitions = aux::apply_t<aux::type_list, transitions_t>;

 private:
  using sm_all_t = aux::apply_t<aux::inherit, aux::join_t<aux::type_list<sm_t>, aux::apply_t<get_sm_t, state_machines>>>;
  using sub_sms_t = aux::apply_t<aux::pool, typename convert_to_sm<TSM, aux::apply_t<get_sub_sms, states>>::type>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using deps_t =
      aux::apply_t<aux::pool,
                   aux::apply_t<aux::unique_t, aux::join_t<deps, logger_dep_t, aux::apply_t<merge_deps, sub_sms_t>>>>;
  struct events_ids : aux::apply_t<aux::inherit, events> {};

 public:
  sm(sm &&) = default;
  sm(const sm &) = delete;
  sm &operator=(const sm &) = delete;

  sm(aux::init, deps_t &deps) : deps_(deps), sub_sms_{deps} { aux::get<sm_impl<TSM>>(sub_sms_).start(deps_, sub_sms_); }

  template <class... TDeps, __BOOST_SML_REQUIRES(aux::is_unique_t<TDeps...>::value)>
  explicit sm(TDeps &&... deps) : deps_{aux::init{}, aux::pool<TDeps...>{deps...}}, sub_sms_{aux::pool<TDeps...>{deps...}} {
    aux::get<sm_impl<TSM>>(sub_sms_).start(deps_, sub_sms_);
  }

  template <class TEvent, __BOOST_SML_REQUIRES(aux::is_base_of<TEvent, events_ids>::value)>
  void process_event(const TEvent &event) {
    aux::get<sm_impl<TSM>>(sub_sms_).process_event(event, deps_, sub_sms_);
  }

  template <class TEvent, __BOOST_SML_REQUIRES(!aux::is_base_of<TEvent, events_ids>::value)>
  void process_event(const TEvent &event) {
    aux::get<sm_impl<TSM>>(sub_sms_).process_event(unexpected_event<_, TEvent>{event}, deps_, sub_sms_);
  }

  template <class T = aux::identity<sm_t>, class TVisitor, __BOOST_SML_REQUIRES(concepts::callable<void, TVisitor>::value)>
  void visit_current_states(const TVisitor &visitor) const {
    using type = typename T::type;
    using sm_impl_t = sm_impl<typename TSM::template rebind<type>>;
    using states_t = typename sm_impl_t::states_t;
    constexpr auto regions = sm_impl_t::regions;
    aux::cget<sm_impl_t>(sub_sms_).visit_current_states(visitor, states_t{}, aux::make_index_sequence<regions>{});
  }

  template <class T = aux::identity<sm_t>, class TState>
  bool is(const TState &) const {
    auto result = false;
    visit_current_states<T>([&](auto state) {
      (void)state;
      result |= aux::is_same<typename TState::type, typename decltype(state)::type>::value;
    });
    return result;
  }

  template <class T = aux::identity<sm_t>, class... TStates,
            __BOOST_SML_REQUIRES(sizeof...(TStates) == sm_impl<typename TSM::template rebind<typename T::type>>::regions)>
  bool is(const TStates &...) const {
    auto result = true;
    auto i = 0;
    using type = typename T::type;
    using sm_impl_t = sm_impl<typename TSM::template rebind<type>>;
    using states_ids_t = typename sm_impl_t::states_ids_t;
    using state_t = typename sm_impl_t::state_t;
    state_t state_ids[] = {aux::get_id<state_t, typename TStates::type>((states_ids_t *)0)...};
    visit_current_states<T>([&](auto state) {
      (void)state;
      result &= (aux::get_id<state_t, typename decltype(state)::type>((states_ids_t *)0) == state_ids[i++]);
    });
    return result;
  }

  template <class T = aux::identity<sm_t>, class... TStates,
            __BOOST_SML_REQUIRES(!aux::is_same<no_policy, typename TSM::testing_policy>::value && aux::always<T>::value)>
  void __set_current_states(const TStates &...) {
    using type = typename T::type;
    using sm_impl_t = sm_impl<typename TSM::template rebind<type>>;
    using states_ids_t = typename sm_impl_t::states_ids_t;
    using state_t = typename sm_impl_t::state_t;
    auto &sm = aux::get<sm_impl<TSM>>(sub_sms_);
    auto region = 0;
    (void)aux::swallow{0,
                       (sm.current_state_[region++] = aux::get_id<state_t, typename TStates::type>((states_ids_t *)0), 0)...};
  }

 private:
  deps_t deps_;
  sub_sms_t sub_sms_;
};

}  // back

#endif
