//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
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
#include "boost/sml/back/queue_handler.hpp"
#include "boost/sml/back/utility.hpp"
#include "boost/sml/concepts/callable.hpp"
#include "boost/sml/concepts/composable.hpp"

#if !defined(BOOST_SML_DISABLE_EXCEPTIONS)                                        // __pph__
#if !(defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND))  // __pph__
#define BOOST_SML_DISABLE_EXCEPTIONS true                                         // __pph__
#else                                                                             // __pph__
#define BOOST_SML_DISABLE_EXCEPTIONS false                                        // __pph__
#endif                                                                            // __pph__
#endif                                                                            // __pph__

namespace back {

template <class TSM>
struct sm_impl : aux::conditional_t<aux::is_empty<typename TSM::sm>::value, aux::none_type, typename TSM::sm> {
  using sm_t = typename TSM::sm;
  using thread_safety_t = typename TSM::thread_safety_policy::type;
  template <class T>
  using defer_queue_t = typename TSM::defer_queue_policy::template rebind<T>;
  using defer_flag_t = typename TSM::defer_queue_policy::flag;
  template <class T>
  using process_queue_t = typename TSM::process_queue_policy::template rebind<T>;
  using logger_t = typename TSM::logger_policy::type;
  using dispatch_t = typename TSM::dispatch_policy;
  using transitions_t = decltype(aux::declval<sm_t>().operator()());
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using states_ids_t = aux::apply_t<aux::type_id, states_t>;
  using initial_states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_initial_states, transitions_t>>;
  using initial_states_ids_t = aux::apply_t<aux::type_id, initial_states_t>;
  using history_states_t = aux::apply_t<get_history_states, transitions_t>;
  using has_history_states =
      aux::integral_constant<bool, aux::size<initial_states_t>::value != aux::size<history_states_t>::value>;
  using sub_internal_events_t = aux::apply_t<aux::unique_t, aux::apply_t<get_sub_internal_events, transitions_t>>;
  using events_t = aux::apply_t<aux::unique_t, aux::join_t<sub_internal_events_t, aux::apply_t<get_all_events, transitions_t>>>;
  using events_ids_t = aux::apply_t<aux::inherit, events_t>;
  using has_unexpected_events = typename aux::is_base_of<unexpected, aux::apply_t<aux::inherit, events_t>>::type;
  using has_entry_exits = typename aux::is_base_of<entry_exit, aux::apply_t<aux::inherit, events_t>>::type;
  using defer_t = defer_queue_t<aux::apply_t<queue_event, events_t>>;
  using process_t = process_queue_t<aux::apply_t<queue_event, events_t>>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using state_t = aux::conditional_t<(aux::size<states_t>::value > 0xFF), unsigned short, aux::byte>;
  static constexpr auto regions = aux::size<initial_states_t>::value;
  static_assert(regions > 0, "At least one initial state is required");
#if !BOOST_SML_DISABLE_EXCEPTIONS  // __pph__
  using exceptions = aux::apply_t<aux::unique_t, aux::apply_t<get_exceptions, events_t>>;
  using has_exceptions = aux::integral_constant<bool, (aux::size<exceptions>::value > 0)>;
#endif  // __pph__
  struct mappings : mappings_t<transitions_t> {};

  template <class TPool>
  sm_impl(aux::init, const TPool &p) : sm_impl{p, aux::is_empty<sm_t>{}} {}

  template <class TPool>
  sm_impl(const TPool &p, aux::false_type) : sm_t{aux::try_get<sm_t>(&p)}, transitions_{(*this)()} {
    initialize(typename sm_impl<TSM>::initial_states_t{});
  }

  template <class TPool>
  sm_impl(const TPool &p, aux::true_type) : transitions_{aux::try_get<sm_t>(&p)()} {
    initialize(typename sm_impl<TSM>::initial_states_t{});
  }

  template <class TEvent, class TDeps, class TSubs>
  bool process_event(const TEvent &event, TDeps &deps, TSubs &subs) {
    bool handled = process_internal_events(event, deps, subs);

    // Repeat internal transition until there is no more to process.
    do {
      do {
        while (process_internal_events(anonymous{}, deps, subs)) {
        }
      } while (process_defer_events(deps, subs, handled, aux::type<defer_queue_t<TEvent>>{}, events_t{}));
    } while (process_queued_events(deps, subs, aux::type<process_queue_t<TEvent>>{}, events_t{}));

    return handled;
  }

  void initialize(const aux::type_list<> &) {}

  template <class TState>
  void initialize(const aux::type_list<TState> &) {
    current_state_[0] = aux::get_id<state_t, TState>((states_ids_t *)0);
  }

  template <class... TStates>
  void initialize(const aux::type_list<TStates...> &) {
    auto region = 0;
#if defined(__cpp_fold_expressions)  // __pph__
    ((current_state_[region++] = aux::get_id<state_t, TStates>((states_ids_t *)0)), ...);
#else   // __pph__
    (void)aux::swallow{0, (current_state_[region++] = aux::get_id<state_t, TStates>((states_ids_t *)0), 0)...};
#endif  // __pph__
  }

  template <class TDeps, class TSubs>
  void start(TDeps &deps, TSubs &subs) {
    process_event(on_entry<_, initial>{}, deps, subs);
  }

  template <class TEvent, class TDeps, class TSubs, class... Ts,
            __BOOST_SML_REQUIRES(!aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &, TDeps &, TSubs &, Ts &&...) {
    return false;
  }

  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &event, TDeps &deps, TSubs &subs) {
    policies::log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS  // __pph__
    return process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                    aux::make_index_sequence<regions>{});
#else   // __pph__
    return process_event_except_imp<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#endif  // __pph__
  }

  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &event, TDeps &deps, TSubs &subs) {
    policies::log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS  // __pph__
    return process_event_impl<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                   aux::make_index_sequence<regions>{});
#else   // __pph__
    return process_event_except_imp<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#endif  // __pph__
  }

  template <class TEvent, class TDeps, class TSubs, class... Ts,
            __BOOST_SML_REQUIRES(!aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_event(const TEvent &, TDeps &, TSubs &, Ts &&...) {
    return false;
  }

  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(!aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value)>
  bool process_internal_generic_event(const TEvent &, TDeps &, TSubs &, state_t &) {
    return false;
  }

  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value)>
  bool process_internal_generic_event(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state) {
    policies::log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS  // __pph__
    return process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                    current_state);
#else   // __pph__
    return process_event_except_imp<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, current_state,
                                                                                          has_exceptions{});
#endif  // __pph__
  }

  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_event(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state) {
    return process_internal_generic_event(event, deps, subs, current_state);
  }

  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_event(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state) {
    policies::log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS  // __pph__
    return process_event_impl<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, states_t{}, current_state)
#else   // __pph__
    return process_event_except_imp<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, current_state,
                                                                                         has_exceptions{})
#endif  // __pph__
           || process_internal_generic_event(event, deps, subs, current_state);
  }

  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &states,
                          aux::index_sequence<0>) {
    const auto lock = thread_safety_.create_lock();
    (void)lock;
    return dispatch_t::template dispatch<0, TMappings>(*this, current_state_[0], event, deps, subs, states);
  }

  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates, int... Ns>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &states,
                          aux::index_sequence<Ns...>) {
    const auto lock = thread_safety_.create_lock();
    (void)lock;

    auto handled = false;
#if defined(__cpp_fold_expressions)  // __pph__
    ((handled |= dispatch_t::template dispatch<0, TMappings>(*this, current_state_[Ns], event, deps, subs, states)), ...);
#else   // __pph__
    (void)aux::swallow{
        0,
        (handled |= dispatch_t::template dispatch<0, TMappings>(*this, current_state_[Ns], event, deps, subs, states), 0)...};
#endif  // __pph__
    return handled;
  }

  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &states,
                          state_t &current_state) {
    const auto lock = thread_safety_.create_lock();
    (void)lock;
    return dispatch_t::template dispatch<0, TMappings>(*this, current_state, event, deps, subs, states);
  }

#if !BOOST_SML_DISABLE_EXCEPTIONS  // __pph__
  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_except_imp(const TEvent &event, TDeps &deps, TSubs &subs, aux::false_type) {
    return process_event_impl<TMappings>(event, deps, subs, states_t{}, aux::make_index_sequence<regions>{});
  }

  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_except_imp(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state, aux::false_type) {
    return process_event_impl<TMappings>(event, deps, subs, states_t{}, current_state);
  }

  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_except_imp(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state,
                                aux::true_type) noexcept {
    try {
      return process_event_impl<TMappings>(event, deps, subs, states_t{}, current_state);
    } catch (...) {
      return process_exception(deps, subs, exceptions{});
    }
  }

  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_except_imp(const TEvent &event, TDeps &deps, TSubs &subs, aux::true_type) {
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
  bool process_defer_events(TDeps &, TSubs &, const bool, const aux::type<no_policy> &, const aux::type_list<TEvents...> &) {
    return false;
  }

  template <class TDeps, class TSubs, class TEvent>
  bool process_event_no_defer(TDeps &deps, TSubs &subs, const void *data) {
    const auto &event = *static_cast<const TEvent *>(data);
    bool handled = process_internal_events(event, deps, subs);

    if (handled && defer_again_) {
      ++defer_it_;
      return false;
    } else {
      defer_.erase(defer_it_);
      defer_it_ = defer_.begin();
      defer_end_ = defer_.end();
    }
    return handled;
  }

  template <class TDeps, class TSubs, class TDeferQueue, class... TEvents>
  bool process_defer_events(TDeps &deps, TSubs &subs, const bool handled, const aux::type<TDeferQueue> &,
                            const aux::type_list<TEvents...> &) {
    bool processed_events = false;
    if (handled) {
      using dispatch_table_t = bool (sm_impl::*)(TDeps &, TSubs &, const void *);
      const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TEvents))] = {
          &sm_impl::process_event_no_defer<TDeps, TSubs, TEvents>...};
      defer_processing_ = true;
      defer_again_ = false;
      defer_it_ = defer_.begin();
      defer_end_ = defer_.end();
      while (defer_it_ != defer_end_) {
        processed_events |= (this->*dispatch_table[defer_it_->id])(deps, subs, defer_it_->data);
        defer_again_ = false;
      }
      defer_processing_ = false;
    }
    return processed_events;
  }

  template <class TDeps, class TSubs, class... TEvents>
  bool process_queued_events(TDeps &, TSubs &, const aux::type<no_policy> &, const aux::type_list<TEvents...> &) {
    return false;
  }

  template <class TDeps, class TSubs, class TEvent>
  bool process_event_no_queue(TDeps &deps, TSubs &subs, const void *data) {
    const auto &event = *static_cast<const TEvent *>(data);
    policies::log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS  // __pph__
    return process_event_impl<get_event_mapping_t<TEvent, mappings>>(event, deps, subs, states_t{},
                                                                     aux::make_index_sequence<regions>{});
#else   // __pph__
    return process_event_except_imp<get_event_mapping_t<TEvent, mappings>>(event, deps, subs, has_exceptions{});
#endif  // __pph__
  }

  template <class TDeps, class TSubs, class TDeferQueue, class... TEvents>
  bool process_queued_events(TDeps &deps, TSubs &subs, const aux::type<TDeferQueue> &, const aux::type_list<TEvents...> &) {
    using dispatch_table_t = bool (sm_impl::*)(TDeps &, TSubs &, const void *);
    const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TEvents))] = {
        &sm_impl::process_event_no_queue<TDeps, TSubs, TEvents>...};
    bool wasnt_empty = !process_.empty();
    while (!process_.empty()) {
      (this->*dispatch_table[process_.front().id])(deps, subs, process_.front().data);
      process_.pop();
    }
    return wasnt_empty;
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
#if defined(__cpp_fold_expressions)  // __pph__
    (dispatch_table[current_state_[Ns]](visitor), ...);
#else   // __pph__
    (void)aux::swallow{0, (dispatch_table[current_state_[Ns]](visitor), 0)...};
#endif  // __pph__
  }

  template <class TVisitor, class TState>
  static void visit_state(const TVisitor &visitor) {
    visitor(aux::string<TState>{});
  }

  bool is_terminated() const { return is_terminated_impl(aux::make_index_sequence<regions>{}); }
  bool is_terminated_impl(aux::index_sequence<0>) const {
    return current_state_[0] == aux::get_id<state_t, terminate_state>((states_ids_t *)0);
  }
  template <int... Ns>
  bool is_terminated_impl(aux::index_sequence<Ns...>) const {
#if defined(__cpp_fold_expressions)  // __pph__
    return ((current_state_[Ns] == aux::get_id<state_t, terminate_state>((states_ids_t *)0)) && ...);
#else   // __pph__
    auto result = true;
    (void)aux::swallow{0, (result &= current_state_[Ns] == aux::get_id<state_t, terminate_state>((states_ids_t *)0))...};
    return result;
#endif  // __pph__
  }

  transitions_t transitions_;
  state_t current_state_[regions];
  thread_safety_t thread_safety_;
  defer_t defer_;
  process_t process_;
  defer_flag_t defer_processing_ = defer_flag_t{};
  defer_flag_t defer_again_ = defer_flag_t{};
  typename defer_t::const_iterator defer_it_;
  typename defer_t::const_iterator defer_end_;
};

template <class TSM>
class sm {
  using sm_t = typename TSM::sm;
  using logger_t = typename TSM::logger_policy::type;
  using logger_dep_t =
      aux::conditional_t<aux::is_same<no_policy, logger_t>::value, aux::type_list<>, aux::type_list<logger_t &>>;
  using transitions_t = decltype(aux::declval<sm_t>().operator()());

  static_assert(concepts::composable<sm_t>::value, "Composable constraint is not satisfied!");

 public:
  using states = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using state_machines = aux::apply_t<get_sub_sms, states>;
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_all_events, transitions_t>>;
  using transitions = aux::apply_t<aux::type_list, transitions_t>;

 private:
  using sm_all_t = aux::apply_t<get_non_empty_t, aux::join_t<aux::type_list<sm_t>, aux::apply_t<get_sm_t, state_machines>>>;
  using sub_sms_t =
      aux::apply_t<aux::pool,
                   typename convert_to_sm<TSM, aux::apply_t<aux::unique_t, aux::apply_t<get_sub_sms, states>>>::type>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using deps_t =
      aux::apply_t<aux::pool,
                   aux::apply_t<aux::unique_t, aux::join_t<deps, sm_all_t, logger_dep_t, aux::apply_t<merge_deps, sub_sms_t>>>>;
  struct events_ids : aux::apply_t<aux::inherit, events> {};

 public:
  sm() : deps_{aux::init{}, aux::pool<>{}}, sub_sms_{aux::pool<>{}} { aux::get<sm_impl<TSM>>(sub_sms_).start(deps_, sub_sms_); }

  template <class TDeps, __BOOST_SML_REQUIRES(!aux::is_same<aux::remove_reference_t<TDeps>, sm>::value)>
  explicit sm(TDeps &&deps) : deps_{aux::init{}, aux::pool<TDeps>{deps}}, sub_sms_{aux::pool<TDeps>{deps}} {
    aux::get<sm_impl<TSM>>(sub_sms_).start(deps_, sub_sms_);
  }

  template <class... TDeps, __BOOST_SML_REQUIRES((sizeof...(TDeps) > 1) && aux::is_unique_t<TDeps...>::value)>
  explicit sm(TDeps &&... deps) : deps_{aux::init{}, aux::pool<TDeps...>{deps...}}, sub_sms_{aux::pool<TDeps...>{deps...}} {
    aux::get<sm_impl<TSM>>(sub_sms_).start(deps_, sub_sms_);
  }

  sm(aux::init, deps_t &deps) : deps_{deps}, sub_sms_{deps} { aux::get<sm_impl<TSM>>(sub_sms_).start(deps_, sub_sms_); }
  sm(const sm &) = default;
  sm(sm &&) = default;

  sm &operator=(const sm &) = default;
  sm &operator=(sm &&) = default;

  template <class TEvent, __BOOST_SML_REQUIRES(aux::is_base_of<TEvent, events_ids>::value)>
  bool process_event(const TEvent &event) {
    return aux::get<sm_impl<TSM>>(sub_sms_).process_event(event, deps_, sub_sms_);
  }

  template <class TEvent, __BOOST_SML_REQUIRES(!aux::is_base_of<TEvent, events_ids>::value)>
  bool process_event(const TEvent &event) {
    return aux::get<sm_impl<TSM>>(sub_sms_).process_event(unexpected_event<_, TEvent>{event}, deps_, sub_sms_);
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
    using type = typename T::type;
    using sm_impl_t = sm_impl<typename TSM::template rebind<type>>;
    using state_t = typename sm_impl_t::state_t;
    using states_ids_t = typename sm_impl_t::states_ids_t;
    return aux::get_id<state_t, typename TState::type>((states_ids_t *)0) == aux::cget<sm_impl_t>(sub_sms_).current_state_[0];
  }

  template <class T = aux::identity<sm_t>, template <class...> class TState>
  bool is(const TState<terminate_state> &) const {
    using type = typename T::type;
    using sm_impl_t = sm_impl<typename TSM::template rebind<type>>;
    using state_t = typename sm_impl_t::state_t;
    using states_ids_t = typename sm_impl_t::states_ids_t;
    auto result = false;
    visit_current_states<T>([&](auto state) {
      (void)state;
      result |= (aux::get_id<state_t, terminate_state>((states_ids_t *)0) ==
                 aux::get_id<state_t, typename decltype(state)::type>((states_ids_t *)0));
    });
    return result;
  }

  template <class T = aux::identity<sm_t>, class... TStates,
            __BOOST_SML_REQUIRES(sizeof...(TStates) == sm_impl<typename TSM::template rebind<typename T::type>>::regions)>
  bool is(const TStates &...) const {
    using type = typename T::type;
    using sm_impl_t = sm_impl<typename TSM::template rebind<type>>;
    using states_ids_t = typename sm_impl_t::states_ids_t;
    using state_t = typename sm_impl_t::state_t;
    auto result = true;
    auto i = 0;
    state_t state_ids[] = {aux::get_id<state_t, typename TStates::type>((states_ids_t *)0)...};
    visit_current_states<T>([&](auto state) {
      (void)state;
      result &= (aux::get_id<state_t, typename decltype(state)::type>((states_ids_t *)0) == state_ids[i++]);
    });
    return result;
  }

  template <class T = aux::identity<sm_t>, class... TStates,
            __BOOST_SML_REQUIRES(!aux::is_same<no_policy, typename TSM::testing_policy>::value && aux::always<T>::value)>
  void set_current_states(const TStates &...) {
    using type = typename T::type;
    using sm_impl_t = sm_impl<typename TSM::template rebind<type>>;
    using states_ids_t = typename sm_impl_t::states_ids_t;
    using state_t = typename sm_impl_t::state_t;
    auto &sm = aux::get<sm_impl_t>(sub_sms_);
    auto region = 0;

#if defined(__cpp_fold_expressions)  // __pph__
    ((sm.current_state_[region++] = aux::get_id<state_t, typename TStates::type>((states_ids_t *)0)), ...);
#else   // __pph__
    (void)aux::swallow{0,
                       (sm.current_state_[region++] = aux::get_id<state_t, typename TStates::type>((states_ids_t *)0), 0)...};
#endif  // __pph__
  }

  template <class T>
  operator T &() {
    return aux::get<sm_impl<typename TSM::template rebind<T>>>(sub_sms_);
  }

  template <class T>
  operator const T &() {
    return aux::cget<sm_impl<typename TSM::template rebind<T>>>(sub_sms_);
  }

 private:
  deps_t deps_;
  sub_sms_t sub_sms_;
};

}  // namespace back

#endif
