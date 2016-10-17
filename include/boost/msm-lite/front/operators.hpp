#ifndef OPERATORS_AUOQ1Q6
#define OPERATORS_AUOQ1Q6

namespace detail {

struct operator_base {};

template <class TEvent>
struct event_type {
  using type = TEvent;
};

template <class TEvent>
struct event_type<exception<TEvent>> {
  using type = TEvent;
};

template <class TEvent>
using event_type_t = typename event_type<TEvent>::type;

template <class, class>
aux::type_list<> args__(...);
template <class T, class>
auto args__(int) -> aux::function_traits_t<T>;
template <class T, class E>
auto args__(int) -> aux::function_traits_t<decltype(&T::template operator() < event_type_t<E>>)>;
template <class T, class>
auto args__(int) -> aux::function_traits_t<decltype(&T::operator())>;
template <class T, class E>
using args_t = decltype(args__<T, E>(0));

template <class T, class TEvent, class TSelf,
          aux::enable_if_t<!aux::is_same<TEvent, aux::remove_reference_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &, TSelf &self) {
  return aux::get<T>(self.deps_);
}
template <class, class TEvent, class TSelf>
decltype(auto) get_arg(const exception<TEvent> &event, TSelf &) {
  return event.exception;
}
template <class T, class TEvent, class TSelf,
          aux::enable_if_t<aux::is_same<TEvent, aux::remove_reference_t<T>>::value, int> = 0>
decltype(auto) get_arg(const TEvent &event, TSelf &) {
  return event;
}
template <class... Ts, class T, class TEvent, class TSelf>
auto call_impl(const aux::type<void> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TSelf &self) {
  object(get_arg<Ts>(event, self)...);
  using sm = typename TSelf::type;
  log_action<typename sm::logger_t, typename sm::sm_raw_t>(typename sm::has_logger{}, self.deps_, object, event);
}
template <class... Ts, class T, class TEvent, class TSelf>
auto call_impl(const aux::type<bool> &, const aux::type_list<Ts...> &, T object, const TEvent &event, TSelf &self) {
  const auto result = object(get_arg<Ts>(event, self)...);
  using sm = typename TSelf::type;
  log_guard<typename sm::logger_t, typename sm::sm_raw_t>(typename sm::has_logger{}, self.deps_, object, event, result);
  return result;
}
template <class... Ts, class T, class TEvent, class TSelf, aux::enable_if_t<!aux::is_base_of<operator_base, T>::value, int> = 0>
auto call_impl(const aux::type_list<Ts...> &args, T object, const TEvent &event, TSelf &self) {
  using result_type = decltype(object(get_arg<Ts>(event, self)...));
  return call_impl(aux::type<result_type>{}, args, object, event, self);
}
template <class... Ts, class T, class TEvent, class TSelf, aux::enable_if_t<aux::is_base_of<operator_base, T>::value, int> = 0>
auto call_impl(const aux::type_list<Ts...> &, T object, const TEvent &event, TSelf &self) {
  return object(event, self);
}
template <class T, class TEvent, class TSelf>
auto call(T object, const TEvent &event, TSelf &self, const aux::false_type &) {
  return call_impl(args_t<T, TEvent>{}, object, event, self);
}
template <class T, class TEvent, class TSelf>
auto call(T object, const TEvent &event, TSelf &self, const aux::true_type &) {
  return object(self, event);
}
template <class T, class TEvent, class TSelf>
auto call(T object, const TEvent &event, TSelf &self) {
  return call(object, event, self, aux::is_base_of<action_base, T>{});
}
template <class... Ts>
class seq_ : operator_base {
 public:
  explicit seq_(Ts... ts) : a(ts...) {}

  template <class TEvent, class TSelf>
  void operator()(const TEvent &event, TSelf &self) {
    for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, self);
  }

 private:
  template <int... Ns, class TEvent, class TSelf>
  void for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSelf &self) {
    int _[]{0, (call(aux::get_by_id<Ns>(a), event, self), 0)...};
    (void)_;
  }

  aux::tuple<Ts...> a;
};
template <class... Ts>
class and_ : operator_base {
 public:
  explicit and_(Ts... ts) : g(ts...) {}

  template <class TEvent, class TSelf>
  auto operator()(const TEvent &event, TSelf &self) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, self);
  }

 private:
  template <int... Ns, class TEvent, class TSelf>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSelf &self) {
    auto result = true;
    int _[]{0, (call(aux::get_by_id<Ns>(g), event, self) ? result : result = false)...};
    (void)_;
    return result;
  }

  aux::tuple<Ts...> g;
};
template <class... Ts>
class or_ : operator_base {
 public:
  explicit or_(Ts... ts) : g(ts...) {}

  template <class TEvent, class TSelf>
  auto operator()(const TEvent &event, TSelf &self) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, self);
  }

 private:
  template <int... Ns, class TEvent, class TSelf>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSelf &self) {
    auto result = false;
    int _[]{0, (call(aux::get_by_id<Ns>(g), event, self) ? result = true : result)...};
    (void)_;
    return result;
  }

  aux::tuple<Ts...> g;
};
template <class T>
class not_ : operator_base {
 public:
  explicit not_(T t) : g(t) {}

  template <class TEvent, class TSelf>
  auto operator()(const TEvent &event, TSelf &self) {
    return !call(g, event, self);
  }

 private:
  T g;
};

}  // detail

template <class T, BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T>::value)>
auto operator!(const T &t) {
  return detail::not_<aux::zero_wrapper<T>>(aux::zero_wrapper<T>{t});
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator&&(const T1 &t1, const T2 &t2) {
  return detail::and_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator||(const T1 &t1, const T2 &t2) {
  return detail::or_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2,
          BOOST_MSM_LITE_REQUIRES(concepts::callable<void, T1>::value &&concepts::callable<void, T2>::value)>
auto operator,(const T1 &t1, const T2 &t2) {
  return detail::seq_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}

#endif
