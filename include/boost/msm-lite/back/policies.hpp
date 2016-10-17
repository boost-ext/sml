#ifndef POLICIES_Y0QZTJTY
#define POLICIES_Y0QZTJTY

namespace detail {

struct thread_safety_policy__ {};
struct defer_queue_policy__ {};
struct logger_policy__ {};

template <class, class, class TDeps, class TEvent>
void log_process_event(const aux::false_type&, TDeps&, const TEvent&) {}

template <class TLogger, class SM, class TDeps, class TEvent>
void log_process_event(const aux::true_type&, TDeps& deps, const TEvent& event) {
  return static_cast<aux::pool_type<TLogger>&>(deps).value.template log_process_event<SM>(event);
}

template <class, class, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::false_type&, TDeps&, const TSrcState&, const TDstState&) {}

template <class TLogger, class SM, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::true_type&, TDeps& deps, const TSrcState& src, const TDstState& dst) {
  return static_cast<aux::pool_type<TLogger>&>(deps).value.template log_state_change<SM>(src, dst);
}

template <class, class, class TDeps, class TAction, class TEvent>
void log_action(const aux::false_type&, TDeps&, const TAction&, const TEvent&) {}

template <class TLogger, class SM, class TDeps, class TAction, class TEvent>
void log_action(const aux::true_type&, TDeps& deps, const TAction& action, const TEvent& event) {
  return static_cast<aux::pool_type<TLogger>&>(deps).value.template log_action<SM>(action, event);
}

template <class, class, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::false_type&, TDeps&, const TGuard&, const TEvent&, bool) {}

template <class TLogger, class SM, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::true_type&, TDeps& deps, const TGuard& guard, const TEvent& event, bool result) {
  return static_cast<aux::pool_type<TLogger>&>(deps).value.template log_guard<SM>(guard, event, result);
}

struct no_policy {
  using type = no_policy;
  template <class>
  using rebind = no_policy;
  aux::byte _[0];
};

}  // detail

#endif
