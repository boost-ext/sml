//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_HPP
#define BOOST_SML_HPP
#if (__cplusplus < 201305L && _MSC_VER < 1900)
#error "[Boost::ext].SML requires C++14 support (Clang-3.4+, GCC-5.1+, MSVC-2015+)"
#else
#define BOOST_SML_VERSION 1'1'6
#define BOOST_SML_NAMESPACE_BEGIN \
  namespace boost {               \
  inline namespace ext {          \
  namespace sml {                 \
  inline namespace v1_1_6 {
#define BOOST_SML_NAMESPACE_END \
  }                             \
  }                             \
  }                             \
  }
#if defined(__clang__)
#define __BOOST_SML_UNUSED __attribute__((unused))
#define __BOOST_SML_VT_INIT \
  {}
#if !defined(BOOST_SML_CFG_DISABLE_MIN_SIZE)
#define __BOOST_SML_ZERO_SIZE_ARRAY(...) __VA_ARGS__ _[0]
#else
#define __BOOST_SML_ZERO_SIZE_ARRAY(...)
#endif
#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...)
#define __BOOST_SML_TEMPLATE_KEYWORD template
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
#pragma clang diagnostic ignored "-Wzero-length-array"
#elif defined(__GNUC__)
#if !defined(__has_builtin)
#define __BOOST_SML_DEFINED_HAS_BUILTIN
#define __has_builtin(...) 0
#endif
#define __BOOST_SML_UNUSED __attribute__((unused))
#define __BOOST_SML_VT_INIT \
  {}
#if !defined(BOOST_SML_CFG_DISABLE_MIN_SIZE)
#define __BOOST_SML_ZERO_SIZE_ARRAY(...) __VA_ARGS__ _[0]
#else
#define __BOOST_SML_ZERO_SIZE_ARRAY(...)
#endif
#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...) __VA_ARGS__ ? __VA_ARGS__ : 1
#define __BOOST_SML_TEMPLATE_KEYWORD template
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#if defined(__GNUC__) && (__GNUC__ >= 10)
#pragma GCC diagnostic ignored "-Wsubobject-linkage"
#endif
#elif defined(_MSC_VER) && !defined(__clang__)
#define __BOOST_SML_DEFINED_HAS_BUILTIN
#define __has_builtin(...) __has_builtin##__VA_ARGS__
#define __has_builtin__make_integer_seq(...) 1
#define __BOOST_SML_UNUSED
#define __BOOST_SML_VT_INIT
#define __BOOST_SML_ZERO_SIZE_ARRAY(...)
#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...) __VA_ARGS__ ? __VA_ARGS__ : 1
#if defined(_MSC_VER) && !defined(__clang__) && _MSC_VER >= 1910  // MSVC 2017
#define __BOOST_SML_TEMPLATE_KEYWORD template
#else
#define __BOOST_SML_TEMPLATE_KEYWORD
#endif
#pragma warning(disable : 4503)
#pragma warning(disable : 4200)
#endif
BOOST_SML_NAMESPACE_BEGIN
#define __BOOST_SML_REQUIRES(...) typename aux::enable_if<__VA_ARGS__, int>::type = 0
namespace aux {
using byte = unsigned char;
struct none_type {};
template <class...>
struct type {};
template <class T, T>
struct non_type {};
template <class, class>
struct pair {};
template <class...>
struct type_list {
  using type = type_list;
};
template <bool...>
struct bool_list {
  using type = bool_list;
};
template <class... Ts>
struct inherit : Ts... {
  using type = inherit;
};
template <class T>
struct identity {
  using type = T;
};
template <class T>
T &&declval();
template <class T, T V>
struct integral_constant {
  using type = integral_constant;
  static constexpr T value = V;
};
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;
template <class... Ts>
using void_t = void;
template <class...>
struct always : true_type {};
template <class...>
struct never : false_type {};
namespace detail {
template <bool>
struct conditional;
template <>
struct conditional<false> {
  template <class, class T>
  using fn = T;
};
template <>
struct conditional<true> {
  template <class T, class>
  using fn = T;
};
}  // namespace detail
template <bool B, class T, class F>
struct conditional {
  using type = typename detail::conditional<B>::template fn<T, F>;
};
template <bool B, class T, class F>
using conditional_t = typename detail::conditional<B>::template fn<T, F>;
template <bool B, class T = void>
struct enable_if {};
template <class T>
struct enable_if<true, T> {
  using type = T;
};
template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;
template <class, class>
struct is_same : false_type {};
template <class T>
struct is_same<T, T> : true_type {};
template <class T, class U>
#if defined(_MSC_VER) && !defined(__clang__)
struct is_base_of : integral_constant<bool, __is_base_of(T, U)> {
};
#else
using is_base_of = integral_constant<bool, __is_base_of(T, U)>;
#endif
template <class T, class... TArgs>
decltype(T(declval<TArgs>()...), true_type{}) test_is_constructible(int);
template <class, class...>
false_type test_is_constructible(...);
template <class T, class... TArgs>
#if defined(_MSC_VER) && !defined(__clang__)
struct is_constructible : decltype(test_is_constructible<T, TArgs...>(0)) {
};
#else
using is_constructible = decltype(test_is_constructible<T, TArgs...>(0));
#endif
template <class T, class U>
struct is_empty_base : T {
  U _;
};
template <class T>
struct is_empty : aux::integral_constant<bool, sizeof(is_empty_base<T, none_type>) == sizeof(none_type)> {};
template <class>
struct function_traits;
template <class R, class... TArgs>
struct function_traits<R (*)(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class... TArgs>
struct function_traits<R(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...) const> {
  using args = type_list<TArgs...>;
};
#if __cplusplus > 201402L && __cpp_noexcept_function_type >= 201510
template <class R, class... TArgs>
struct function_traits<R (*)(TArgs...) noexcept> {
  using args = type_list<TArgs...>;
};
template <class R, class... TArgs>
struct function_traits<R(TArgs...) noexcept> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...) noexcept> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...) const noexcept> {
  using args = type_list<TArgs...>;
};
#endif
template <class T>
using function_traits_t = typename function_traits<T>::args;
template <class T>
struct remove_const {
  using type = T;
};
template <class T>
struct remove_const<const T> {
  using type = T;
};
template <class T>
using remove_const_t = typename remove_const<T>::type;
template <class T>
struct remove_reference {
  using type = T;
};
template <class T>
struct remove_reference<T &> {
  using type = T;
};
template <class T>
struct remove_reference<T &&> {
  using type = T;
};
template <class T>
using remove_reference_t = typename remove_reference<T>::type;
}  // namespace aux
namespace aux {
using swallow = int[];
template <int...>
struct index_sequence {
  using type = index_sequence;
};
#if __has_builtin(__make_integer_seq)
template <class T, T...>
struct integer_sequence;
template <int... Ns>
struct integer_sequence<int, Ns...> {
  using type = index_sequence<Ns...>;
};
template <int N>
struct make_index_sequence_impl {
  using type = typename __make_integer_seq<integer_sequence, int, N>::type;
};
#else
template <class, class>
struct concat;
template <int... I1, int... I2>
struct concat<index_sequence<I1...>, index_sequence<I2...>> : index_sequence<I1..., (sizeof...(I1) + I2)...> {};
template <int N>
struct make_index_sequence_impl
    : concat<typename make_index_sequence_impl<N / 2>::type, typename make_index_sequence_impl<N - N / 2>::type>::type {};
template <>
struct make_index_sequence_impl<0> : index_sequence<> {};
template <>
struct make_index_sequence_impl<1> : index_sequence<0> {};
#endif
template <int N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;
template <class...>
struct join {
  using type = type_list<>;
};
template <class T>
struct join<T> {
  using type = T;
};
template <class... Ts>
struct join<type_list<Ts...>> : type_list<Ts...> {};
template <class... T1s, class... T2s>
struct join<type_list<T1s...>, type_list<T2s...>> : type_list<T1s..., T2s...> {};
template <class... T1s, class... T2s, class... T3s>
struct join<type_list<T1s...>, type_list<T2s...>, type_list<T3s...>> : type_list<T1s..., T2s..., T3s...> {};
template <class... T1s, class... T2s, class... Ts>
struct join<type_list<T1s...>, type_list<T2s...>, Ts...> : join<type_list<T1s..., T2s...>, Ts...> {};
template <class... Ts, class... T1s, class... T2s, class... T3s, class... T4s, class... T5s, class... T6s, class... T7s,
          class... T8s, class... T9s, class... T10s, class... T11s, class... T12s, class... T13s, class... T14s, class... T15s,
          class... T16s, class... Us>
struct join<type_list<Ts...>, type_list<T1s...>, type_list<T2s...>, type_list<T3s...>, type_list<T4s...>, type_list<T5s...>,
            type_list<T6s...>, type_list<T7s...>, type_list<T8s...>, type_list<T9s...>, type_list<T10s...>, type_list<T11s...>,
            type_list<T12s...>, type_list<T13s...>, type_list<T14s...>, type_list<T15s...>, type_list<T16s...>, Us...>
    : join<type_list<Ts..., T1s..., T2s..., T3s..., T4s..., T5s..., T6s..., T7s..., T8s..., T9s..., T10s..., T11s..., T12s...,
                     T13s..., T14s..., T15s..., T16s...>,
           Us...> {};
template <class... TArgs>
using join_t = typename join<TArgs...>::type;
template <class, class...>
struct unique_impl;
template <class T, class... Rs, class... Ts>
struct unique_impl<type<Rs...>, T, Ts...> : conditional_t<is_base_of<type<T>, inherit<type<Rs>...>>::value,
                                                          unique_impl<type<Rs...>, Ts...>, unique_impl<type<Rs..., T>, Ts...>> {
};
template <class... Rs>
struct unique_impl<type<Rs...>> : type_list<Rs...> {};
template <class... Ts>
struct unique : unique_impl<type<>, Ts...> {};
template <class T>
struct unique<T> : type_list<T> {};
template <class... Ts>
using unique_t = typename unique<Ts...>::type;
template <class...>
struct is_unique;
template <class T>
struct is_unique<T> : true_type {};
template <class T, class... Rs, class... Ts>
struct is_unique<type<Rs...>, T, Ts...>
    : conditional_t<is_base_of<type<T>, inherit<type<Rs>...>>::value, false_type, is_unique<type<Rs..., T>, Ts...>> {};
template <class... Ts>
using is_unique_t = is_unique<type<>, Ts...>;
template <template <class...> class, class>
struct apply;
template <template <class...> class T, template <class...> class U, class... Ts>
struct apply<T, U<Ts...>> {
  using type = T<Ts...>;
};
template <template <class...> class T, class D>
using apply_t = typename apply<T, D>::type;
template <int, class T>
struct tuple_type {
  explicit tuple_type(const T &object) : value(object) {}
  T value;
};
template <class, class...>
struct tuple_impl;
template <int... Ns, class... Ts>
struct tuple_impl<index_sequence<Ns...>, Ts...> : tuple_type<Ns, Ts>... {
  explicit tuple_impl(Ts... ts) : tuple_type<Ns, Ts>(ts)... {}
};
template <>
struct tuple_impl<index_sequence<0>> {
  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};
template <class... Ts>
using tuple = tuple_impl<make_index_sequence<sizeof...(Ts)>, Ts...>;
template <int N, class T>
T &get_by_id(tuple_type<N, T> *object) {
  return static_cast<tuple_type<N, T> &>(*object).value;
}
struct init {};
struct pool_type_base {
  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};
template <class T, class = void>
struct pool_type_impl : pool_type_base {
  explicit pool_type_impl(T object) : value{object} {}
  template <class TObject>
  pool_type_impl(init i, TObject object) : value{i, object} {}
  T value;
};
template <class T>
struct pool_type_impl<T &, aux::enable_if_t<aux::is_constructible<T>::value && aux::is_constructible<T, T>::value>>
    : pool_type_base {
  explicit pool_type_impl(T &value) : value{value} {}
  template <class TObject>
  explicit pool_type_impl(TObject value) : value_{value}, value{value_} {}
  template <class TObject>
  pool_type_impl(const init &i, const TObject &object) : value(i, object) {}
  T value_{};
  T &value;
};
template <class T>
struct pool_type : pool_type_impl<T> {
  using pool_type_impl<T>::pool_type_impl;
};
template <class T>
struct missing_ctor_parameter {
  static constexpr auto value = false;
  auto operator()() const { return T{}(); }
  template <class U, __BOOST_SML_REQUIRES(!aux::is_base_of<pool_type_base, U>::value && aux::is_constructible<U>::value)>
  operator U() {
    return {};
  }
#if !(defined(_MSC_VER) && !defined(__clang__))
  template <class TMissing, __BOOST_SML_REQUIRES(!aux::is_base_of<pool_type_base, TMissing>::value)>
  operator TMissing &() const {
    static_assert(missing_ctor_parameter<TMissing>::value,
                  "State Machine is missing a constructor parameter! Check out the `missing_ctor_parameter` error to see the "
                  "missing type.");
  }
#endif
};
template <class T>
missing_ctor_parameter<T> try_get(...) {
  return {};
}
template <class T>
T try_get(const pool_type<T> *object) {
  return object->value;
}
template <class T>
const T &try_get(const pool_type<const T &> *object) {
  return object->value;
}
template <class T>
T &try_get(const pool_type<T &> *object) {
  return object->value;
}
template <class T, class TPool>
T &get(TPool &p) {
  return static_cast<pool_type<T> &>(p).value;
}
template <class T, class TPool>
const T &cget(const TPool &p) {
  return static_cast<const pool_type<T> &>(p).value;
}
template <class... Ts>
struct pool : pool_type<Ts>... {
  using boost_di_inject__ = type_list<Ts...>;
  pool() = default;
  explicit pool(Ts... ts) : pool_type<Ts>(ts)... {}
  template <class... TArgs>
  pool(init, const pool<TArgs...> &p) : pool_type<Ts>(try_get<aux::remove_const_t<aux::remove_reference_t<Ts>>>(&p))... {}
  template <class... TArgs>
  pool(const pool<TArgs...> &p) : pool_type<Ts>(init{}, p)... {}
};
template <>
struct pool<> {
  using boost_di_inject__ = type_list<>;
  pool() = default;
  template <class... Ts>
  explicit pool(Ts &&...) {}
  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};
template <int, class>
struct type_id_type {};
template <class, class...>
struct type_id_impl;
template <int... Ns, class... Ts>
struct type_id_impl<index_sequence<Ns...>, Ts...> : type_id_type<Ns, Ts>... {};
template <class... Ts>
struct type_id : type_id_impl<make_index_sequence<sizeof...(Ts)>, Ts...> {};
template <class R, class T, int N>
constexpr R get_id(type_id_type<N, T> *) {
  return static_cast<R>(N);
}
template <template <class...> class, class T>
struct is : false_type {};
template <template <class...> class T, class... Ts>
struct is<T, T<Ts...>> : true_type {};
template <class>
struct size;
template <template <class...> class T, class... Ts>
struct size<T<Ts...>> {
  static constexpr auto value = sizeof...(Ts);
};
template <int... Ts>
constexpr int max_element() {
  int max = 0;
  (void)swallow{0, (Ts > max ? max = Ts : max)...};
  return max;
}
template <class TExpr, class = void>
struct zero_wrapper : TExpr {
  using type = TExpr;
  explicit zero_wrapper(const TExpr &expr) : TExpr(expr) {}
  const TExpr &get() const { return *this; }
};
template <class R, class TBase, class... TArgs, class T>
struct zero_wrapper<R (TBase::*)(TArgs...), T> {
  explicit zero_wrapper(R (TBase::*ptr)(TArgs...)) : ptr{ptr} {}
  auto operator()(TBase &self, TArgs... args) { return (self.*ptr)(args...); }

 private:
  R (TBase::*ptr)(TArgs...){};
};
template <class R, class TBase, class... TArgs, class T>
struct zero_wrapper<R (TBase::*)(TArgs...) const, T> {
  explicit zero_wrapper(R (TBase::*ptr)(TArgs...) const) : ptr{ptr} {}
  auto operator()(TBase &self, TArgs... args) { return (self.*ptr)(args...); }

 private:
  R (TBase::*ptr)(TArgs...) const {};
};
template <class R, class... TArgs, class T>
struct zero_wrapper<R (*)(TArgs...), T> {
  explicit zero_wrapper(R (*ptr)(TArgs...)) : ptr{ptr} {}
  auto operator()(TArgs... args) { return (*ptr)(args...); }

 private:
  R (*ptr)(TArgs...){};
};
#if defined(__cpp_noexcept_function_type)
template <class R, class TBase, class... TArgs, class T>
struct zero_wrapper<R (TBase::*)(TArgs...) noexcept, T> {
  explicit zero_wrapper(R (TBase::*ptr)(TArgs...) noexcept) : ptr{ptr} {}
  auto operator()(TBase &self, TArgs... args) { return (self.*ptr)(args...); }

 private:
  R (TBase::*ptr)(TArgs...) noexcept {};
};
template <class R, class TBase, class... TArgs, class T>
struct zero_wrapper<R (TBase::*)(TArgs...) const noexcept, T> {
  explicit zero_wrapper(R (TBase::*ptr)(TArgs...) const noexcept) : ptr{ptr} {}
  auto operator()(TBase &self, TArgs... args) { return (self.*ptr)(args...); }

 private:
  R (TBase::*ptr)(TArgs...) const noexcept {};
};
template <class R, class... TArgs, class T>
struct zero_wrapper<R (*)(TArgs...) noexcept, T> {
  explicit zero_wrapper(R (*ptr)(TArgs...) noexcept) : ptr{ptr} {}
  auto operator()(TArgs... args) { return (*ptr)(args...); }

 private:
  R (*ptr)(TArgs...) noexcept {};
};
#endif
template <class, class>
struct zero_wrapper_impl;
template <class TExpr, class... TArgs>
struct zero_wrapper_impl<TExpr, type_list<TArgs...>> {
  auto operator()(TArgs... args) const { return reinterpret_cast<const TExpr &>(*this)(args...); }
  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};
template <class TExpr>
struct zero_wrapper<TExpr, void_t<decltype(+declval<TExpr>())>>
    : zero_wrapper_impl<TExpr, function_traits_t<decltype(&TExpr::operator())>> {
  using type = TExpr;
  template <class... Ts>
  zero_wrapper(Ts &&...) {}
  const TExpr &get() const { return reinterpret_cast<const TExpr &>(*this); }
};
namespace detail {
template <class, int N, int... Ns>
auto get_type_name(const char *ptr, index_sequence<Ns...>) {
  static const char str[] = {ptr[N + Ns]..., 0};
  return str;
}
}  // namespace detail
template <class T>
const char *get_type_name() {
#if defined(_MSC_VER) && !defined(__clang__)
  return detail::get_type_name<T, 39>(__FUNCSIG__, make_index_sequence<sizeof(__FUNCSIG__) - 39 - 8>{});
#elif defined(__clang__) && (__clang_major__ >= 12)
  return detail::get_type_name<T, 50>(__PRETTY_FUNCTION__, make_index_sequence<sizeof(__PRETTY_FUNCTION__) - 50 - 2>{});
#elif defined(__clang__)
  return detail::get_type_name<T, 63>(__PRETTY_FUNCTION__, make_index_sequence<sizeof(__PRETTY_FUNCTION__) - 63 - 2>{});
#elif defined(__GNUC__)
  return detail::get_type_name<T, 68>(__PRETTY_FUNCTION__, make_index_sequence<sizeof(__PRETTY_FUNCTION__) - 68 - 2>{});
#endif
}
#if defined(__cpp_nontype_template_parameter_class)
template <auto N>
struct fixed_string {
  static constexpr auto size = N;
  char data[N + 1]{};
  constexpr fixed_string(char const *str) {
    for (auto i = 0; i < N; ++i) {
      data[i] = str[i];
    }
  }
};
template <auto N>
fixed_string(char const (&)[N]) -> fixed_string<N - 1>;
#endif
template <class T, T...>
struct string;
template <char... Chrs>
struct string<char, Chrs...> {
  using type = string;
  static auto c_str() {
    static constexpr char str[] = {Chrs..., 0};
    return str;
  }
};
template <class T>
struct string<T> {
  using type = T;
  static auto c_str() { return c_str_impl((T *)0); }
  template <class U>
  static decltype(U::c_str()) c_str_impl(U *) {
    return U::c_str();
  }
  static auto c_str_impl(...) { return get_type_name<T>(); }
};
}  // namespace aux
template <class T>
constexpr auto wrap(T callback) {
  return aux::zero_wrapper<T, T>{callback};
}
namespace back {
namespace policies {
struct defer_queue_policy__ {};
template <template <class...> class T>
struct defer_queue : aux::pair<back::policies::defer_queue_policy__, defer_queue<T>> {
  template <class U>
  using rebind = T<U>;
  using flag = bool;
};
}  // namespace policies
}  // namespace back
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
  queue_event(T object) {
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
  explicit on_entry(const TEvent &event = {}) : event_(event) {}
  const TEvent &event_;
};
template <class T, class TEvent = T>
struct on_exit : internal_event, entry_exit {
  static auto c_str() { return "on_exit"; }
  explicit on_exit(const TEvent &event = {}) : event_(event) {}
  const TEvent &event_;
};
template <class T, class TException = T>
struct exception : internal_event {
  using type = TException;
  explicit exception(const TException &exception = {}) : exception_(exception) {}
  const TException &exception_;
};
template <class T, class TEvent = T>
struct unexpected_event : internal_event, unexpected {
  explicit unexpected_event(const TEvent &event = {}) : event_(event) {}
  const TEvent &event_;
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
namespace back {
template <class>
class sm;
template <class>
struct sm_impl;
template <class, class...>
struct sm_policy;
template <class TEvent>
using get_event = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<>, aux::type_list<TEvent>>;
template <class, class, class TEvent>
struct get_all_events_impl {
  using type = get_event<TEvent>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<TSrc, TDst, unexpected_event<TEvent>> {
  using type = aux::type_list<TEvent>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<sm<TSrc>, TDst, TEvent> {
  using type = aux::join_t<get_event<TEvent>, typename sm<TSrc>::events>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<TSrc, sm<TDst>, TEvent> {
  using type = aux::join_t<get_event<TEvent>, typename sm<TDst>::events>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<sm<TSrc>, sm<TDst>, TEvent> {
  using type = aux::join_t<get_event<TEvent>, typename sm<TSrc>::events, typename sm<TDst>::events>;
};
template <class, class TEvent>
struct get_sub_internal_events_impl {
  using type = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<TEvent>, aux::type_list<>>;
};
template <class T, class TEvent>
struct get_sub_internal_events_impl<sm<T>, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm_impl<T>::sub_internal_events_t>;
};
template <class... Ts>
using get_all_events =
    aux::join_t<typename get_all_events_impl<typename Ts::src_state, typename Ts::dst_state, typename Ts::event>::type...>;
template <class... Ts>
using get_sub_internal_events =
    aux::join_t<typename get_sub_internal_events_impl<typename Ts::src_state, typename Ts::event>::type...,
                typename get_sub_internal_events_impl<typename Ts::dst_state, typename Ts::event>::type...>;
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
    aux::join_t<typename aux::conditional<Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>::type...>;
template <class... Ts>
using get_history_states = aux::join_t<
    typename aux::conditional<!Ts::history && Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>::type...>;
template <class>
struct get_sub_sm : aux::type_list<> {};
template <class T>
struct get_sub_sm<sm<T>> : aux::join_t<aux::type_list<T>, typename sm<T>::state_machines> {};
template <class... Ts>
using get_sub_sms = aux::join_t<typename get_sub_sm<Ts>::type...>;
template <class... Ts>
using get_sm_t = aux::type_list<typename Ts::sm...>;
template <class... Ts>
using get_non_empty_t =
    aux::join_t<typename aux::conditional<aux::is_empty<Ts>::value, aux::type_list<>, aux::type_list<Ts>>::type...>;
template <class... Ts>
using merge_deps = aux::join_t<typename Ts::deps...>;
template <class>
struct sub_sm;
template <class T>
struct sub_sm<sm_impl<sm_policy<T>>> {
  template <class U, class... TPolicies>
  static sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>> &get(
      aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> *object) {
    return static_cast<aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> &>(*object).value;
  }
  template <class... TPolicies>
  static sm_impl<sm_policy<T, TPolicies...>> &get(aux::pool_type<sm_impl<sm_policy<T, TPolicies...>>> *object) {
    return static_cast<aux::pool_type<sm_impl<sm_policy<T, TPolicies...>>> &>(*object).value;
  }
  template <class... TPolicies>
  static const sm_impl<sm_policy<T, TPolicies...>> &cget(const aux::pool_type<sm_impl<sm_policy<T, TPolicies...>>> *object) {
    return static_cast<const aux::pool_type<sm_impl<sm_policy<T, TPolicies...>>> &>(*object).value;
  }
};
template <class T, class U>
struct sub_sm<sm_impl<sm_policy<T, aux::identity<U>>>> {
  template <class... TPolicies>
  static sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>> &get(
      aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> *object) {
    return static_cast<aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> &>(*object).value;
  }
  template <class... TPolicies>
  static const sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>> &cget(
      const aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> *object) {
    return static_cast<const aux::pool_type<sm_impl<sm_policy<T, aux::identity<U>, TPolicies...>>> &>(*object).value;
  }
};
template <class T, class... TPolicies>
struct rebind_impl {
  using type = sm_policy<T, TPolicies...>;
};
template <class T, class... TDetails, class... TPolicies>
struct rebind_impl<sm_policy<T, TDetails...>, TPolicies...> {
  using type = sm_policy<T, TDetails..., TPolicies...>;
};
template <class T, class... TDetails, class... TPolicies>
struct rebind_impl<sm<sm_policy<T, TDetails...>>, TPolicies...> {
  using type = sm_policy<T, TDetails..., TPolicies...>;
};
template <class, class>
struct convert_to_sm;
template <class T, class... Ts>
struct convert_to_sm<T, aux::type_list<Ts...>> {
  using type = aux::type_list<sm_impl<T>, sm_impl<typename T::template rebind<Ts>>...>;
};
}  // namespace back
namespace back {
template <class>
class sm;
template <class>
struct sm_impl;
template <class...>
struct transitions;
template <class...>
struct transitions_sub;
template <class T, class... Ts>
struct transitions<T, Ts...> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    if (aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state, typename SM::has_entry_exits{})) {
      return true;
    }
    return transitions<Ts...>::execute(event, sm, deps, subs, current_state);
  }
};
template <class T>
struct transitions<T> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    return aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state, typename SM::has_entry_exits{});
  }
};
template <>
struct transitions<aux::true_type> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    sm.process_internal_event(unexpected_event<TEvent>{event}, deps, subs, current_state);
    return false;
  }
};
template <>
struct transitions<aux::false_type> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &, SM &, TDeps &, TSubs &, typename SM::state_t &) {
    return false;
  }
};
template <class TSM, class T, class... Ts>
struct transitions_sub<sm<TSM>, T, Ts...> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    return execute_impl(event, sm, deps, subs, current_state);
  }
  template <class, class SM, class TDeps, class TSubs>
  static bool execute(const anonymous &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    if (sub_sm<sm_impl<TSM>>::cget(&subs).is_terminated()) {
      const auto handled = sub_sm<sm_impl<TSM>>::get(&subs).process_event(event, deps, subs);
      return handled ? handled : transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
    }
    return false;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute_impl(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    const auto handled = sub_sm<sm_impl<TSM>>::get(&subs).process_event(event, deps, subs);
    return handled ? handled : transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
  }
  template <class _, class TEvent, class SM, class TDeps, class TSubs>
  static bool execute_impl(const back::on_entry<_, TEvent> &event, SM &sm, TDeps &deps, TSubs &subs,
                           typename SM::state_t &current_state) {
    transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
    sub_sm<sm_impl<TSM>>::get(&subs).process_event(event, deps, subs);
    return true;
  }
  template <class _, class TEvent, class SM, class TDeps, class TSubs>
  static bool execute_impl(const back::on_exit<_, TEvent> &event, SM &sm, TDeps &deps, TSubs &subs,
                           typename SM::state_t &current_state) {
    sub_sm<sm_impl<TSM>>::get(&subs).process_event(event, deps, subs);
    transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
    return true;
  }
};
template <class TSM>
struct transitions_sub<sm<TSM>> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &, TDeps &deps, TSubs &subs, typename SM::state_t &) {
    return sub_sm<sm_impl<TSM>>::get(&subs).process_event(event, deps, subs);
  }
  template <class, class SM, class TDeps, class TSubs>
  static bool execute(const anonymous &, SM &, TDeps &, TSubs &, typename SM::state_t &) {
    return false;
  }
};
}  // namespace back
namespace back {
template <class>
class sm;
template <class>
struct state;
template <class>
struct event;
template <class...>
struct transitions;
template <class...>
struct transitions_sub;
template <class, class>
struct state_mappings;
template <class S, class... Ts>
struct state_mappings<S, aux::type_list<Ts...>> {
  using element_type = state<S>;
  using types = aux::type_list<Ts...>;
};
template <class, class>
struct event_mappings;
template <class E, class... Ts>
struct event_mappings<E, aux::inherit<Ts...>> {
  using element_type = event<E>;
  using types = aux::type_list<Ts...>;
};
template <class...>
struct unique_mappings;
template <class, class...>
struct unique_mappings_impl;
template <class... Ts>
using unique_mappings_t = typename unique_mappings<Ts...>::type;
template <class, class, class, class R>
struct get_mapping : aux::type_list<R> {};
template <class E, class T, class R>
struct get_mapping<event<E>, event<E>, T, R>
    : aux::type_list<event_mappings<E, aux::apply_t<unique_mappings_t, aux::join_t<typename R::types, typename T::types>>>> {};
template <class S, class T, class R>
struct get_mapping<state<S>, state<S>, T, R>
    : aux::type_list<state_mappings<S, aux::join_t<typename R::types, typename T::types>>> {};
template <class T, class... Ts>
struct extend_mapping : aux::join_t<typename get_mapping<typename T::element_type, typename Ts::element_type, T, Ts>::type...> {
};
template <class T, class... Ts>
using extend_mapping_t = aux::apply_t<aux::inherit, typename extend_mapping<T, Ts...>::type>;
template <bool, class, class...>
struct conditional_mapping;
template <class T1, class T2, class... Rs, class... Ts>
struct conditional_mapping<true, aux::type<T1, aux::inherit<Rs...>>, T2, Ts...> {
  using type = unique_mappings_impl<aux::type<aux::inherit<T1>, extend_mapping_t<T2, Rs...>>, Ts...>;
};
template <class T1, class T2, class... Rs, class... Ts>
struct conditional_mapping<false, aux::type<T1, aux::inherit<Rs...>>, T2, Ts...> {
  using type =
      unique_mappings_impl<aux::type<aux::inherit<T1, aux::type<typename T2::element_type>>, aux::inherit<T2, Rs...>>, Ts...>;
};
template <class T1, class T2, class... Rs, class... Ts>
struct unique_mappings_impl<aux::type<T1, aux::inherit<Rs...>>, T2, Ts...>
    : conditional_mapping<aux::is_base_of<aux::type<typename T2::element_type>, T1>::value, aux::type<T1, aux::inherit<Rs...>>,
                          T2, Ts...>::type {};
template <class T1, class Rs>
struct unique_mappings_impl<aux::type<T1, Rs>> : aux::apply_t<aux::inherit, Rs> {};
template <class... Ts>
struct unique_mappings : unique_mappings_impl<aux::type<aux::none_type, aux::inherit<>>, Ts...> {};
template <class T>
struct unique_mappings<T> : aux::inherit<T> {};
template <class, class...>
struct mappings;
template <class... Ts>
struct mappings<aux::pool<Ts...>>
    : unique_mappings_t<
          event_mappings<typename Ts::event, aux::inherit<state_mappings<typename Ts::src_state, aux::type_list<Ts>>>>...> {};
template <class T>
using mappings_t = typename mappings<T>::type;
template <class, class TUnexpected>
transitions<TUnexpected> get_state_mapping_impl(...);
template <class T, class, class... Ts>
transitions<Ts...> get_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);
template <class T, class TMappings, class TUnexpected>
struct get_state_mapping {
  using type = decltype(get_state_mapping_impl<T, TUnexpected>((TMappings *)0));
};
template <class S>
transitions_sub<S> get_sub_state_mapping_impl(...);
template <class T, class... Ts>
transitions_sub<T, Ts...> get_sub_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);
template <class T, class TMappings, class TUnexpected>
struct get_state_mapping<sm<T>, TMappings, TUnexpected> {
  using type = decltype(get_sub_state_mapping_impl<sm<T>>((TMappings *)0));
};
template <class T, class TMappings, class TUnexpected>
using get_state_mapping_t = typename get_state_mapping<T, TMappings, TUnexpected>::type;
template <class>
transitions<aux::true_type> get_event_mapping_impl(...);
template <class T, class TMappings>
TMappings get_event_mapping_impl(event_mappings<T, TMappings> *);
template <class T, class... T1Mappings, class... T2Mappings>
unique_mappings_t<T1Mappings..., T2Mappings...> get_event_mapping_impl(event_mappings<T, aux::inherit<T1Mappings...>> *,
                                                                       event_mappings<_, aux::inherit<T2Mappings...>> *);
template <class T, class TMappings>
struct get_event_mapping_impl_helper
    : aux::conditional<aux::is_same<transitions<aux::true_type>, decltype(get_event_mapping_impl<_>((TMappings *)0))>::value,
                       decltype(get_event_mapping_impl<T>((TMappings *)0)),
                       decltype(get_event_mapping_impl<T>((TMappings *)0, (TMappings *)0))>::type {};
template <class T, class TMappings>
struct get_event_mapping_impl_helper<exception<T>, TMappings> : decltype(get_event_mapping_impl<exception<T>>((TMappings *)0)) {
};
template <class T1, class T2, class TMappings>
struct get_event_mapping_impl_helper<unexpected_event<T1, T2>, TMappings>
    : decltype(get_event_mapping_impl<unexpected_event<T1, T2>>((TMappings *)0)) {};
template <class T1, class T2, class TMappings>
struct get_event_mapping_impl_helper<on_entry<T1, T2>, TMappings>
    : decltype(get_event_mapping_impl<on_entry<T1, T2>>((TMappings *)0)) {};
template <class T1, class T2, class TMappings>
struct get_event_mapping_impl_helper<on_exit<T1, T2>, TMappings>
    : decltype(get_event_mapping_impl<on_exit<T1, T2>>((TMappings *)0)) {};
template <class T, class TMappings>
using get_event_mapping_t = get_event_mapping_impl_helper<T, TMappings>;
}  // namespace back
namespace back {
namespace policies {
struct dispatch_policy__ {};
template <class T>
struct dispatch : aux::pair<dispatch_policy__, T> {
  using type = T;
};
struct jump_table {
  template <int, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs>
  static bool dispatch(sm_impl &, State &, const TEvent &, TDeps &, TSubs &, const aux::type_list<> &) {
    return false;
  }
  template <int, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class... TStates>
  static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TStates...> &) {
    using dispatch_table_t = bool (*)(const TEvent &, sm_impl &, TDeps &, TSubs &, State &);
    constexpr static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &get_state_mapping_t<TStates, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl,
                                                                                                            TDeps, TSubs>...};
    return dispatch_table[current_state](event, self, deps, subs, current_state);
  }
};
struct branch_stm {
  template <int, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs>
  static bool dispatch(sm_impl &, State &, const TEvent &, TDeps &, TSubs &, const aux::type_list<> &) {
    return false;
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState,
            class... TStates>
  static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState, TStates...> &) {
    return current_state == N
               ? get_state_mapping_t<TState, TMappings, typename sm_impl::has_unexpected_events>::template execute<
                     TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state)
               : dispatch<N + 1, TMappings>(self, current_state, event, deps, subs, aux::type_list<TStates...>{});
  }
};
struct switch_stm {
  template <int, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs>
  static bool dispatch(sm_impl &, State &, const TEvent &, TDeps &, TSubs &, const aux::type_list<> &) {
    return false;
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState,
            class... TStates>
  static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState, TStates...> &) {
    switch (current_state) {
      case N:
        return get_state_mapping_t<TState, TMappings, typename sm_impl::has_unexpected_events>::template execute<
            TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
      default:
        return dispatch<N + 1, TMappings>(self, current_state, event, deps, subs, aux::type_list<TStates...>{});
    }
  }
};
#if defined(__cpp_fold_expressions)
struct fold_expr {
  template <class TMappings, int... Ns, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class... TStates>
  static bool dispatch_impl(sm_impl &self, State &current_state, aux::index_sequence<Ns...>, const TEvent &event, TDeps &deps,
                            TSubs &subs, const aux::type_list<TStates...> &) {
    return ((current_state == Ns
                 ? get_state_mapping_t<TStates, TMappings, typename sm_impl::has_unexpected_events>::template execute<
                       TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state)
                 : false) ||
            ...);
  }
  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class... TStates>
  static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TStates...> &states) {
    return dispatch_impl<TMappings>(self, current_state, aux::make_index_sequence<sizeof...(TStates)>{}, event, deps, subs,
                                    states);
  }
};
#endif
}  // namespace policies
}  // namespace back
namespace back {
template <class>
class sm;
template <class, class...>
struct sm_policy;
struct no_policy;
namespace policies {
struct logger_policy__ {};
template <class T>
struct logger : aux::pair<logger_policy__, logger<T>> {
  using type = T;
};
template <class>
struct get_state_name;
template <class T>
struct get_state_name<aux::string<T>> {
  using type = aux::string<T>;
};
template <class T, class... Ts>
struct get_state_name<aux::string<sm<sm_policy<T, Ts...>>>> {
  using type = typename get_state_name<aux::string<sm_policy<T, Ts...>>>::type;
};
template <class T>
struct get_state_name<aux::string<sm_policy<T>>> {
  using type = aux::string<T>;
};
template <class T, class TName>
struct get_state_name<aux::string<sm_policy<T, TName>>> {
  using type = aux::string<T(typename TName::type)>;
};
template <class T>
using get_state_name_t = typename get_state_name<T>::type;
template <class, class TDeps, class TEvent>
void log_process_event(const aux::type<no_policy> &, TDeps &, const TEvent &) {}
template <class SM, class TLogger, class TDeps, class TEvent>
void log_process_event(const aux::type<TLogger> &, TDeps &deps, const TEvent &event) {
  return static_cast<aux::pool_type<TLogger &> &>(deps).value.template log_process_event<SM>(event);
}
template <class, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::type<no_policy> &, TDeps &, const TSrcState &, const TDstState &) {}
template <class SM, class TLogger, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::type<TLogger> &, TDeps &deps, const TSrcState &, const TDstState &) {
  return static_cast<aux::pool_type<TLogger &> &>(deps).value.template log_state_change<SM>(get_state_name_t<TSrcState>{},
                                                                                            get_state_name_t<TDstState>{});
}
template <class, class TDeps, class TAction, class TEvent>
void log_action(const aux::type<no_policy> &, TDeps &, const TAction &, const TEvent &) {}
template <class SM, class TLogger, class TDeps, class TAction, class TEvent>
void log_action(const aux::type<TLogger> &, TDeps &deps, const TAction &action, const TEvent &event) {
  return static_cast<aux::pool_type<TLogger &> &>(deps).value.template log_action<SM>(action, event);
}
template <class SM, class TLogger, class TDeps, class TAction, class TEvent>
void log_action(const aux::type<TLogger> &, TDeps &deps, const aux::zero_wrapper<TAction> &action, const TEvent &event) {
  return static_cast<aux::pool_type<TLogger &> &>(deps).value.template log_action<SM>(action.get(), event);
}
template <class, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::type<no_policy> &, TDeps &, const TGuard &, const TEvent &, bool) {}
template <class SM, class TLogger, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::type<TLogger> &, TDeps &deps, const TGuard &guard, const TEvent &event, bool result) {
  return static_cast<aux::pool_type<TLogger &> &>(deps).value.template log_guard<SM>(guard, event, result);
}
template <class SM, class TLogger, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::type<TLogger> &, TDeps &deps, const aux::zero_wrapper<TGuard> &guard, const TEvent &event,
               bool result) {
  return static_cast<aux::pool_type<TLogger &> &>(deps).value.template log_guard<SM>(guard.get(), event, result);
}
}  // namespace policies
}  // namespace back
namespace back {
namespace policies {
struct process_queue_policy__ {};
template <template <class...> class T>
struct process_queue : aux::pair<back::policies::process_queue_policy__, process_queue<T>> {
  template <class U>
  using rebind = T<U>;
};
}  // namespace policies
}  // namespace back
namespace back {
namespace policies {
struct testing_policy__ {};
struct testing : aux::pair<testing_policy__, testing> {};
}  // namespace policies
}  // namespace back
namespace back {
namespace policies {
struct thread_safety_policy__ {
  auto create_lock() { return *this; }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
template <class TLock>
struct thread_safe : aux::pair<thread_safety_policy__, thread_safe<TLock>> {
  using type = thread_safe;
  auto create_lock() {
    struct lock_guard {
      explicit lock_guard(TLock &lock) : lock_{lock} { lock_.lock(); }
      ~lock_guard() { lock_.unlock(); }
      TLock &lock_;
    };
    return lock_guard{lock};
  }
  TLock lock;
};
}  // namespace policies
}  // namespace back
namespace back {
struct no_policy : policies::thread_safety_policy__ {
  using type = no_policy;
  template <class>
  using rebind = no_policy;
  template <class...>
  using defer = no_policy;
  using const_iterator = no_policy;
  using flag = no_policy;
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
template <class TDefault, class>
TDefault get_policy(...);
template <class, class T, class TPolicy>
TPolicy get_policy(aux::pair<T, TPolicy> *);
template <class SM, class... TPolicies>
struct sm_policy {
  static_assert(aux::is_same<aux::remove_reference_t<SM>, SM>::value, "SM type can't have qualifiers");
  using sm = SM;
  using thread_safety_policy =
      decltype(get_policy<no_policy, policies::thread_safety_policy__>((aux::inherit<TPolicies...> *)0));
  using defer_queue_policy = decltype(get_policy<no_policy, policies::defer_queue_policy__>((aux::inherit<TPolicies...> *)0));
  using process_queue_policy =
      decltype(get_policy<no_policy, policies::process_queue_policy__>((aux::inherit<TPolicies...> *)0));
  using logger_policy = decltype(get_policy<no_policy, policies::logger_policy__>((aux::inherit<TPolicies...> *)0));
  using testing_policy = decltype(get_policy<no_policy, policies::testing_policy__>((aux::inherit<TPolicies...> *)0));
  using default_dispatch_policy = policies::jump_table;
  using dispatch_policy =
      decltype(get_policy<default_dispatch_policy, policies::dispatch_policy__>((aux::inherit<TPolicies...> *)0));
  template <class T>
  using rebind = typename rebind_impl<T, TPolicies...>::type;
};
}  // namespace back
namespace concepts {
struct callable_fallback {
  void operator()();
};
template <class T>
aux::false_type test_callable(aux::non_type<void (callable_fallback::*)(), &T::operator()> *);
template <class>
aux::true_type test_callable(...);
template <class, class T>
struct callable
    : decltype(test_callable<aux::inherit<aux::conditional_t<__is_class(T), T, aux::none_type>, callable_fallback>>(0)) {};
template <class T, class R, class TBase, class... TArgs>
struct callable<T, R (TBase::*)(TArgs...)> : aux::true_type {};
template <class T, class R, class TBase, class... TArgs>
struct callable<T, R (TBase::*)(TArgs...) const> : aux::true_type {};
#if defined(__cpp_noexcept_function_type)
template <class T, class R, class TBase, class... TArgs>
struct callable<T, R (TBase::*)(TArgs...) noexcept> : aux::true_type {};
template <class T, class R, class TBase, class... TArgs>
struct callable<T, R (TBase::*)(TArgs...) const noexcept> : aux::true_type {};
#endif
}  // namespace concepts
namespace concepts {
template <class T>
decltype(aux::declval<T>().operator()()) composable_impl(int);
template <class>
void composable_impl(...);
template <class T>
struct composable : aux::is<aux::pool, decltype(composable_impl<T>(0))> {};
}  // namespace concepts
#if !defined(BOOST_SML_DISABLE_EXCEPTIONS)
#if !(defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND))
#define BOOST_SML_DISABLE_EXCEPTIONS true
#else
#define BOOST_SML_DISABLE_EXCEPTIONS false
#endif
#endif
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
#if !BOOST_SML_DISABLE_EXCEPTIONS
  using exceptions = aux::apply_t<aux::unique_t, aux::apply_t<get_exceptions, events_t>>;
  using has_exceptions = aux::integral_constant<bool, (aux::size<exceptions>::value > 0)>;
#endif
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
#if defined(__cpp_fold_expressions)
    ((current_state_[region++] = aux::get_id<state_t, TStates>((states_ids_t *)0)), ...);
#else
    (void)aux::swallow{0, (current_state_[region++] = aux::get_id<state_t, TStates>((states_ids_t *)0), 0)...};
#endif
  }
  template <class TDeps, class TSubs>
  void start(TDeps &deps, TSubs &subs) {
    process_event(on_entry<_, initial>{}, deps, subs);
  }
  template <class TEvent, class TDeps, class TSubs, class... Ts,
            __BOOST_SML_REQUIRES(!aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_same<get_event_t<TEvent>, initial>::value)>
  bool process_internal_events(const TEvent &, TDeps &, TSubs &, Ts &&...) {
    return false;
  }
  template <class TEvent, class TDeps, class TSubs, class... Ts,
            __BOOST_SML_REQUIRES(!aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value &&
                                 aux::is_same<get_event_t<TEvent>, initial>::value)>
  bool process_internal_events(const TEvent &event, TDeps &deps, TSubs &subs) {
    policies::log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS
    return process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                    aux::make_index_sequence<regions>{});
#else
    return process_event_except_imp<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#endif
  }
  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &event, TDeps &deps, TSubs &subs) {
    policies::log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS
    return process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                    aux::make_index_sequence<regions>{});
#else
    return process_event_except_imp<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#endif
  }
  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &event, TDeps &deps, TSubs &subs) {
    policies::log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if BOOST_SML_DISABLE_EXCEPTIONS
    return process_event_impl<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                   aux::make_index_sequence<regions>{});
#else
    return process_event_except_imp<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#endif
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
#if BOOST_SML_DISABLE_EXCEPTIONS
    return process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                    current_state);
#else
    return process_event_except_imp<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, current_state,
                                                                                          has_exceptions{});
#endif
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
#if BOOST_SML_DISABLE_EXCEPTIONS
    return process_event_impl<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, states_t{}, current_state)
#else
    return process_event_except_imp<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, current_state,
                                                                                         has_exceptions{})
#endif
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
#if defined(__cpp_fold_expressions)
    ((handled |= dispatch_t::template dispatch<0, TMappings>(*this, current_state_[Ns], event, deps, subs, states)), ...);
#else
    (void)aux::swallow{
        0,
        (handled |= dispatch_t::template dispatch<0, TMappings>(*this, current_state_[Ns], event, deps, subs, states), 0)...};
#endif
    return handled;
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &states,
                          state_t &current_state) {
    const auto lock = thread_safety_.create_lock();
    (void)lock;
    return dispatch_t::template dispatch<0, TMappings>(*this, current_state, event, deps, subs, states);
  }
#if !BOOST_SML_DISABLE_EXCEPTIONS
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
#endif
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
#if BOOST_SML_DISABLE_EXCEPTIONS
    return process_event_impl<get_event_mapping_t<TEvent, mappings>>(event, deps, subs, states_t{},
                                                                     aux::make_index_sequence<regions>{});
#else
    return process_event_except_imp<get_event_mapping_t<TEvent, mappings>>(event, deps, subs, has_exceptions{});
#endif
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
#if defined(__cpp_fold_expressions)
    (dispatch_table[current_state_[Ns]](visitor), ...);
#else
    (void)aux::swallow{0, (dispatch_table[current_state_[Ns]](visitor), 0)...};
#endif
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
#if defined(__cpp_fold_expressions)
    return ((current_state_[Ns] == aux::get_id<state_t, terminate_state>((states_ids_t *)0)) && ...);
#else
    auto result = true;
    (void)aux::swallow{0, (result &= current_state_[Ns] == aux::get_id<state_t, terminate_state>((states_ids_t *)0))...};
    return result;
#endif
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
    auto result = false;
    visit_current_states<T>([&](auto state) {
      (void)state;
      result |= (aux::get_id<state_t, typename TState::type>((states_ids_t *)0) ==
                 aux::get_id<state_t, typename decltype(state)::type>((states_ids_t *)0));
    });
    return result;
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
#if defined(__cpp_fold_expressions)
    ((sm.current_state_[region++] = aux::get_id<state_t, typename TStates::type>((states_ids_t *)0)), ...);
#else
    (void)aux::swallow{0,
                       (sm.current_state_[region++] = aux::get_id<state_t, typename TStates::type>((states_ids_t *)0), 0)...};
#endif
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
namespace front {
struct operator_base {};
struct action_base {};
template <class TRootSM, class... TSubSMs>
TRootSM get_root_sm_impl(aux::pool<TRootSM, TSubSMs...> *);
template <class TSubs>
using get_root_sm_t = decltype(get_root_sm_impl((TSubs *)0));
template <class, class>
aux::type_list<action_base> args1__(...);
template <class T, class E>
auto args1__(int) -> aux::function_traits_t<decltype(&T::operator())>;
template <class T, class E>
auto args__(int) -> aux::function_traits_t<decltype(&T::__BOOST_SML_TEMPLATE_KEYWORD operator()<back::get_event_t<E>>)>;
template <class T, class E>
auto args__(...) -> decltype(args1__<T, E>(0));
template <class T, class E>
using args_t = decltype(args__<T, E>(0));
template <class T, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<T> &, const TEvent &, TSM &, TDeps &deps) {
  return aux::get<T>(deps);
}
template <class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<TEvent> &, const TEvent &event, TSM &, TDeps &) {
  return event;
}
template <class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const TEvent &event, TSM &, TDeps &) {
  return event;
}
template <class T, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::unexpected_event<T, TEvent> &event, TSM &, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::on_entry<T, TEvent> &event, TSM &, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::on_exit<T, TEvent> &event, TSM &, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::exception<T, TEvent> &event, TSM &, TDeps &) {
  return event.exception_;
}
template <class... TEvents, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<back::defer<TEvents...>> &, const TEvent, TSM &sm, TDeps &) {
  return back::defer<TEvents...>{sm.defer_};
}
template <class... TEvents, class TEvent, class TSM, class TDeps>
decltype(auto) get_arg(const aux::type<back::process<TEvents...>> &, const TEvent, TSM &sm, TDeps &) {
  return back::process<TEvents...>{sm.process_};
}
template <class, class, class>
struct call;
template <class TEvent>
struct call<TEvent, aux::type_list<>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &, TSM &, TDeps &, TSubs &) {
    return object();
  }
};
template <class TEvent, class TLogger>
struct call<TEvent, aux::type_list<>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &, TDeps &deps, TSubs &) {
    using result_type = decltype(object());
    return execute_impl<typename TSM::sm_t>(aux::type<result_type>{}, object, event, deps);
  }
  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<bool> &, T object, const TEvent &event, TDeps &deps) {
    const auto result = object();
    back::policies::log_guard<TSM>(aux::type<TLogger>{}, deps, object, event, result);
    return result;
  }
  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<void> &, T object, const TEvent &event, TDeps &deps) {
    back::policies::log_action<TSM>(aux::type<TLogger>{}, deps, object, event);
    object();
  }
};
template <class TEvent>
struct call<TEvent, aux::type_list<TEvent>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &, TDeps &, TSubs &) {
    return object(event);
  }
};
template <class TEvent, class TLogger>
struct call<TEvent, aux::type_list<TEvent>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &, TDeps &deps, TSubs &) {
    using result_type = decltype(object(event));
    return execute_impl<typename TSM::sm_t>(aux::type<result_type>{}, object, event, deps);
  }
  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<bool> &, T object, const TEvent &event, TDeps &deps) {
    const auto result = object(event);
    back::policies::log_guard<TSM>(aux::type<TLogger>{}, deps, object, event, result);
    return result;
  }
  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<void> &, T object, const TEvent &event, TDeps &deps) {
    back::policies::log_action<TSM>(aux::type<TLogger>{}, deps, object, event);
    object(event);
  }
};
template <class TEvent>
struct call<TEvent, aux::type_list<action_base>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return object(event, sm, deps, subs);
  }
};
template <class TEvent, class TLogger>
struct call<TEvent, aux::type_list<action_base>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return object(event, sm, deps, subs);
  }
};
template <class TEvent, class... Ts>
struct call<TEvent, aux::type_list<Ts...>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &) {
    return object(get_arg(aux::type<Ts>{}, event, sm, deps)...);
  }
};
template <class TEvent, class... Ts, class TLogger>
struct call<TEvent, aux::type_list<Ts...>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &) {
    using result_type = decltype(object(get_arg(aux::type<Ts>{}, event, sm, deps)...));
    return execute_impl<typename TSM::sm_t>(aux::type<result_type>{}, object, event, sm, deps);
  }
  template <class TSM, class T, class SM, class TDeps>
  static auto execute_impl(const aux::type<bool> &, T object, const TEvent &event, SM &sm, TDeps &deps) {
    const auto result = object(get_arg(aux::type<Ts>{}, event, sm, deps)...);
    back::policies::log_guard<TSM>(aux::type<TLogger>{}, deps, object, event, result);
    return result;
  }
  template <class TSM, class T, class SM, class TDeps>
  static auto execute_impl(const aux::type<void> &, T object, const TEvent &event, SM &sm, TDeps &deps) {
    back::policies::log_action<TSM>(aux::type<TLogger>{}, deps, object, event);
    object(get_arg(aux::type<Ts>{}, event, sm, deps)...);
  }
};
template <class... Ts>
class seq_ : operator_base {
 public:
  explicit seq_(Ts... ts) : a(ts...) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  void operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps, subs);
  }

 private:
  template <int... Ns, class TEvent, class TSM, class TDeps, class TSubs>
  void for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
#if defined(__cpp_fold_expressions)
    (call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<Ns>(&a), event, sm, deps, subs), ...);
#else
    (void)aux::swallow{
        0, (call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<Ns>(&a), event, sm, deps, subs),
            0)...};
#endif
  }
  aux::tuple<Ts...> a;
};
template <class... Ts>
class and_ : operator_base {
 public:
  explicit and_(Ts... ts) : g(ts...) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps, subs);
  }

 private:
  template <int... Ns, class TEvent, class TSM, class TDeps, class TSubs>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
#if defined(__cpp_fold_expressions)
    return (call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<Ns>(&g), event, sm, deps, subs) &&
            ...);
#else
    auto result = true;
    (void)aux::swallow{0, (result = result && call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(
                                                  aux::get_by_id<Ns>(&g), event, sm, deps, subs),
                           0)...};
    return result;
#endif
  }
  aux::tuple<Ts...> g;
};
template <class... Ts>
class or_ : operator_base {
 public:
  explicit or_(Ts... ts) : g(ts...) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps, subs);
  }

 private:
  template <int... Ns, class TEvent, class TSM, class TDeps, class TSubs>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
#if defined(__cpp_fold_expressions)
    return (call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<Ns>(&g), event, sm, deps, subs) ||
            ...);
#else
    auto result = false;
    (void)aux::swallow{0, (result = result || call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(
                                                  aux::get_by_id<Ns>(&g), event, sm, deps, subs),
                           0)...};
    return result;
#endif
  }
  aux::tuple<Ts...> g;
};
template <class T>
class not_ : operator_base {
 public:
  explicit not_(T t) : g(t) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return !call<TEvent, args_t<T, TEvent>, typename TSM::logger_t>::execute(g, event, sm, deps, subs);
  }

 private:
  T g;
};
}  // namespace front
template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
auto operator!(const T &t) {
  return front::not_<aux::zero_wrapper<T>>(aux::zero_wrapper<T>{t});
}
template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator&&(const T1 &t1, const T2 &t2) {
  return front::and_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator||(const T1 &t1, const T2 &t2) {
  return front::or_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<void, T1>::value &&concepts::callable<void, T2>::value)>
auto operator,(const T1 &t1, const T2 &t2) {
  return front::seq_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
namespace front {
namespace actions {
struct defer : action_base {
  template <class TEvent, class TSM, class TDeps, class TSubs>
  void operator()(const TEvent &event, TSM &sm, TDeps &, TSubs &) {
    if (sm.defer_processing_) {
      sm.defer_again_ = true;
    } else {
      sm.defer_.push_back(event);
    }
  }
};
}  // namespace actions
}  // namespace front
using testing = back::policies::testing;
template <class T>
using logger = back::policies::logger<T>;
template <class T>
using thread_safe = back::policies::thread_safe<T>;
template <class T>
using dispatch = back::policies::dispatch<T>;
template <template <class...> class T>
using defer_queue = back::policies::defer_queue<T>;
template <template <class...> class T>
using process_queue = back::policies::process_queue<T>;
#if defined(_MSC_VER) && !defined(__clang__)
template <class T, class... TPolicies, class T__ = aux::remove_reference_t<decltype(aux::declval<T>())>>
using sm = back::sm<back::sm_policy<T__, TPolicies...>>;
#else
template <class T, class... TPolicies>
using sm = back::sm<back::sm_policy<T, TPolicies...>>;
#endif
#if defined(__cpp_deduction_guides)
namespace front {
template <class T, class... TPolicies>
struct sm : back::sm<back::sm_policy<T, TPolicies...>> {
  constexpr sm(T t) : back::sm<back::sm_policy<T, TPolicies...>>::sm{t} {}
  using back::sm<back::sm_policy<T, TPolicies...>>::sm;
};
}  // namespace front
#endif
namespace concepts {
aux::false_type transitional_impl(...);
template <class T>
auto transitional_impl(T &&t) -> aux::always<typename T::dst_state, typename T::src_state, typename T::event, typename T::deps,
                                             decltype(T::initial), decltype(T::history)>;
template <class T>
struct transitional : decltype(transitional_impl(aux::declval<T>())) {};
}  // namespace concepts
namespace front {
namespace actions {
struct process {
  template <class TEvent>
  class process_impl : public action_base {
   public:
    explicit process_impl(const TEvent &event) : event(event) {}
    template <class T, class TSM, class TDeps, class TSubs>
    void operator()(const T &, TSM &, TDeps &, TSubs &subs) {
      aux::get<get_root_sm_t<TSubs>>(subs).process_.push(event);
    }

   private:
    TEvent event;
  };
  template <class TEvent>
  auto operator()(const TEvent &event) {
    return process_impl<TEvent>{event};
  }
};
}  // namespace actions
}  // namespace front
namespace front {
template <class, class>
struct transition_eg;
template <class, class>
struct transition_ea;
template <class TEvent>
struct event {
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const {
    return transition_eg<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const {
    return transition_ea<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }
  auto operator()() const { return TEvent{}; }
};
}  // namespace front
namespace front {
struct initial_state {};
struct history_state {};
template <class...>
struct transition;
template <class, class>
struct transition_sa;
template <class, class>
struct transition_sg;
template <class, class>
struct transition_eg;
template <class>
struct state;
template <class TState>
struct state_impl {
  template <class T>
  auto operator<=(const T &t) const {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T>
  auto operator+(const T &t) const {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const {
    return transition_sg<TState, aux::zero_wrapper<T>>{static_cast<const TState &>(*this), aux::zero_wrapper<T>{t}};
  }
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const {
    return transition_sa<TState, aux::zero_wrapper<T>>{static_cast<const TState &>(*this), aux::zero_wrapper<T>{t}};
  }
};
template <class TState>
struct state : state_impl<state<TState>> {
  using type = TState;
  static constexpr auto initial = false;
  static constexpr auto history = false;
  auto operator*() const { return state<TState(initial_state)>{}; }
  auto operator()(const initial_state &) const { return state<TState(initial_state)>{}; }
  auto operator()(const history_state &) const { return state<TState(history_state)>{}; }
  template <class... Ts>
  auto operator()(const state<Ts> &...) const {
    return state<TState(Ts...)>{};
  }
  template <class T>
  auto operator=(const T &t) const {
    return transition<T, state>{t, *this};
  }
  template <class T>
  auto sm() const {
    return state<back::sm<back::sm_policy<T, aux::identity<TState>>>>{};
  }
};
template <class TState>
struct state<TState(initial_state)> : state_impl<state<TState(initial_state)>> {
  using type = TState;
  static constexpr auto initial = true;
  static constexpr auto history = false;
  template <class T>
  auto operator=(const T &t) const {
    return transition<T, state>{t, *this};
  }
};
template <class TState>
struct state<TState(history_state)> : state_impl<state<TState(history_state)>> {
  using type = TState;
  static constexpr auto initial = true;
  static constexpr auto history = true;
  template <class T>
  auto operator=(const T &t) const {
    return transition<T, state>{t, *this};
  }
};
#if defined(_MSC_VER) && !defined(__clang__)
template <class T, class T__ = aux::remove_reference_t<decltype(aux::declval<T>())>, class = void>
struct state_sm {
  using type = state<T>;
};
template <class T, class T__>
struct state_sm<T, T__, aux::enable_if_t<concepts::composable<T__>::value>> {
  using type = state<back::sm<back::sm_policy<T__>>>;
};
#else
template <class T, class = void>
struct state_sm {
  using type = state<T>;
};
template <class T>
struct state_sm<T, aux::enable_if_t<concepts::composable<T>::value>> {
  using type = state<back::sm<back::sm_policy<T>>>;
};
#endif
}  // namespace front
namespace front {
struct internal {};
template <class, class>
struct ignore;
template <class E, class... Ts>
struct ignore<E, aux::type_list<Ts...>> {
  template <class T>
  struct non_events {
    using type =
        aux::conditional_t<aux::is_same<back::get_event_t<E>, aux::remove_const_t<aux::remove_reference_t<T>>>::value ||
                               aux::is_same<T, action_base>::value,
                           aux::type_list<>, aux::type_list<T>>;
  };
  using type = aux::join_t<typename non_events<Ts>::type...>;
};
template <class T, class E, class = void>
struct get_deps {
  using type = typename ignore<E, args_t<T, E>>::type;
};
template <class T, class E>
using get_deps_t = typename get_deps<T, E>::type;
template <template <class...> class T, class... Ts, class E>
struct get_deps<T<Ts...>, E, aux::enable_if_t<aux::is_base_of<operator_base, T<Ts...>>::value>> {
  using type = aux::join_t<get_deps_t<Ts, E>...>;
};
struct always {
  using type = always;
  bool operator()() const { return true; }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
struct none {
  using type = none;
  void operator()() {}
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
template <class...>
struct transition;
template <class E, class G>
struct transition<front::event<E>, G> {
  template <class T>
  auto operator/(const T &t) const {
    return transition<front::event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
  }
  front::event<E> e;
  G g;
};
template <class E, class G, class A>
struct transition<front::event<E>, G, A> {
  front::event<E> e;
  G g;
  A a;
};
template <class S2, class G, class A>
struct transition<state<S2>, G, A> : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A> {
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A>::g;
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A>::a;
  transition(const G &g, const A &a) : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A>{g, a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<back::anonymous>, G, A>{g, a};
  }
  const auto &operator()() const { return *this; }
  template <class TEvent, class TSM, class TDeps, class TSubs>
  auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) -> void {
    typename TSM::state_t s{};
    this->execute(event, sm, deps, subs, s);
  }
};
template <class S1, class S2>
struct transition<state<S1>, state<S2>> : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, none> {
  transition(const state<S1> &, const state<S2> &)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, none>{always{}, none{}} {}
};
template <class S2, class G>
struct transition_sg<state<S2>, G> : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, none> {
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, G, none>::g;
  transition_sg(const state<S2> &, const G &g)
      : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, none>{g, none{}} {}
  template <class T>
  auto operator/(const T &t) const {
    return transition<state<S2>, G, aux::zero_wrapper<T>>{g, aux::zero_wrapper<T>{t}};
  }
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<back::anonymous>, G, none>{g, none{}};
  }
};
template <class S2, class A>
struct transition_sa<state<S2>, A> : transition<state<internal>, state<S2>, front::event<back::anonymous>, always, A> {
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, always, A>::a;
  transition_sa(const state<S2> &, const A &a)
      : transition<state<internal>, state<S2>, front::event<back::anonymous>, always, A>{always{}, a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<back::anonymous>, always, A>{always{}, a};
  }
};
template <class S2, class E>
struct transition<state<S2>, front::event<E>> {
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<E>, always, none>{always{}, none{}};
  }
  const state<S2> &s2;
  front::event<E> e;
};
template <class E, class G>
struct transition_eg<front::event<E>, G> {
  template <class T>
  auto operator/(const T &t) const {
    return transition<front::event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
  }
  front::event<E> e;
  G g;
};
template <class E, class A>
struct transition_ea<front::event<E>, A> {
  front::event<E> e;
  A a;
};
template <class S1, class S2, class G, class A>
struct transition<state<S1>, transition<state<S2>, G, A>>
    : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, A> {
  transition(const state<S1> &, const transition<state<S2>, G, A> &t)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, A>{t.g, t.a} {}
};
template <class S1, class E, class G, class A>
struct transition<state<S1>, transition<front::event<E>, G, A>>
    : transition<state<internal>, state<S1>, front::event<E>, G, A> {
  using transition<state<internal>, state<S1>, front::event<E>, G, A>::g;
  using transition<state<internal>, state<S1>, front::event<E>, G, A>::a;
  transition(const state<S1> &, const transition<front::event<E>, G, A> &t)
      : transition<state<internal>, state<S1>, front::event<E>, G, A>{t.g, t.a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S1>, front::event<E>, G, A>{g, a};
  }
};
template <class S1, class S2, class E>
struct transition<state<S1>, transition<state<S2>, front::event<E>>>
    : transition<state<S1>, state<S2>, front::event<E>, always, none> {
  transition(const state<S1> &, const transition<state<S2>, front::event<E>> &)
      : transition<state<S1>, state<S2>, front::event<E>, always, none>{always{}, none{}} {}
};
template <class S1, class S2, class G>
struct transition<state<S1>, transition_sg<state<S2>, G>>
    : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, none> {
  transition(const state<S1> &, const transition_sg<state<S2>, G> &t)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, none>{t.g, none{}} {}
};
template <class S1, class S2, class A>
struct transition<state<S1>, transition_sa<state<S2>, A>>
    : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, A> {
  transition(const state<S1> &, const transition_sa<state<S2>, A> &t)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, A>{always{}, t.a} {}
};
template <class S2, class E, class G>
struct transition<state<S2>, transition_eg<front::event<E>, G>>
    : transition<state<internal>, state<S2>, front::event<E>, G, none> {
  using transition<state<internal>, state<S2>, front::event<E>, G, none>::g;
  transition(const state<S2> &, const transition_eg<front::event<E>, G> &t)
      : transition<state<internal>, state<S2>, front::event<E>, G, none>{t.g, none{}} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<E>, G, none>{g, none{}};
  }
};
template <class S1, class S2, class E, class G>
struct transition<state<S1>, transition<state<S2>, transition_eg<front::event<E>, G>>>
    : transition<state<S1>, state<S2>, front::event<E>, G, none> {
  transition(const state<S1> &, const transition<state<S2>, transition_eg<front::event<E>, G>> &t)
      : transition<state<S1>, state<S2>, front::event<E>, G, none>{t.g, none{}} {}
};
template <class S2, class E, class A>
struct transition<state<S2>, transition_ea<front::event<E>, A>>
    : transition<state<internal>, state<S2>, front::event<E>, always, A> {
  using transition<state<internal>, state<S2>, front::event<E>, always, A>::a;
  transition(const state<S2> &, const transition_ea<front::event<E>, A> &t)
      : transition<state<internal>, state<S2>, front::event<E>, always, A>{always{}, t.a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<E>, always, A>{always{}, a};
  }
};
template <class S1, class S2, class E, class A>
struct transition<state<S1>, transition<state<S2>, transition_ea<front::event<E>, A>>>
    : transition<state<S1>, state<S2>, front::event<E>, always, A> {
  transition(const state<S1> &, const transition<state<S2>, transition_ea<front::event<E>, A>> &t)
      : transition<state<S1>, state<S2>, front::event<E>, always, A>{always{}, t.a} {}
};
template <class S1, class S2, class E, class G, class A>
struct transition<state<S1>, transition<state<S2>, transition<front::event<E>, G, A>>>
    : transition<state<S1>, state<S2>, front::event<E>, G, A> {
  transition(const state<S1> &, const transition<state<S2>, transition<front::event<E>, G, A>> &t)
      : transition<state<S1>, state<S2>, front::event<E>, G, A>{t.g, t.a} {}
};
template <class T, class TSubs, class... Ts, class... THs>
void update_composite_states(TSubs &subs, aux::true_type, const aux::type_list<THs...> &) {
  using state_t = typename T::state_t;
  auto &sm = back::sub_sm<T>::get(&subs);
  (void)sm;
#if defined(__cpp_fold_expressions)
  ((sm.current_state_[aux::get_id<state_t, THs>((typename T::initial_states_ids_t *)0)] =
        aux::get_id<state_t, THs>((typename T::states_ids_t *)0)),
   ...);
#else
  (void)aux::swallow{0, (sm.current_state_[aux::get_id<state_t, THs>((typename T::initial_states_ids_t *)0)] =
                             aux::get_id<state_t, THs>((typename T::states_ids_t *)0),
                         0)...};
#endif
}
template <class T, class TSubs, class... Ts>
void update_composite_states(TSubs &subs, aux::false_type, Ts &&...) {
  back::sub_sm<T>::get(&subs).initialize(typename T::initial_states_t{});
}
template <class SM, class TDeps, class TSubs, class TSrcState, class TDstState>
void update_current_state(SM &, TDeps &deps, TSubs &, typename SM::state_t &current_state,
                          const typename SM::state_t &new_state, const TSrcState &, const TDstState &) {
  back::policies::log_state_change<typename SM::sm_t>(aux::type<typename SM::logger_t>{}, deps,
                                                      aux::string<typename TSrcState::type>{},
                                                      aux::string<typename TDstState::type>{});
  current_state = new_state;
}
template <class SM, class TDeps, class TSubs, class TSrcState, class T>
void update_current_state(SM &, TDeps &deps, TSubs &subs, typename SM::state_t &current_state,
                          const typename SM::state_t &new_state, const TSrcState &, const state<back::sm<T>> &) {
  back::policies::log_state_change<typename SM::sm_t>(aux::type<typename SM::logger_t>{}, deps,
                                                      aux::string<typename TSrcState::type>{}, aux::string<T>{});
  current_state = new_state;
  update_composite_states<back::sm_impl<T>>(subs, typename back::sm_impl<T>::has_history_states{},
                                            typename back::sm_impl<T>::history_states_t{});
}
template <class S1, class S2, class E, class G, class A>
struct transition<state<S1>, state<S2>, front::event<E>, G, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = G;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;
  transition(const G &g, const A &a) : g(g), a(a) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
      update_current_state(sm, deps, subs, current_state,
                           aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                           state<dst_state>{});
      call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
      sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
      return true;
    }
    return false;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      update_current_state(sm, deps, subs, current_state,
                           aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                           state<dst_state>{});
      call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
      return true;
    }
    return false;
  }
  G g;
  A a;
};
template <class S2, class E, class G, class A>
struct transition<state<internal>, state<S2>, front::event<E>, G, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = G;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;
  transition(const G &g, const A &a) : g(g), a(a) {}
  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, Ts &&...) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
      return true;
    }
    return false;
  }
  G g;
  A a;
};
template <class S1, class S2, class E, class A>
struct transition<state<S1>, state<S2>, front::event<E>, always, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = always;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;
  transition(const always &, const A &a) : a(a) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
    update_current_state(sm, deps, subs, current_state,
                         aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                         state<dst_state>{});
    call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
    sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
    return true;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    update_current_state(sm, deps, subs, current_state,
                         aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                         state<dst_state>{});
    call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
    return true;
  }
  A a;
};
template <class S2, class E, class A>
struct transition<state<internal>, state<S2>, front::event<E>, always, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = always;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;
  transition(const always &, const A &a) : a(a) {}
  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, Ts &&...) {
    call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
    return true;
  }
  A a;
};
template <class S1, class S2, class E, class G>
struct transition<state<S1>, state<S2>, front::event<E>, G, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = G;
  using action = none;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<G, E>>;
  transition(const G &g, const none &) : g(g) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
      update_current_state(sm, deps, subs, current_state,
                           aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                           state<dst_state>{});
      sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
      return true;
    }
    return false;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      update_current_state(sm, deps, subs, current_state,
                           aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                           state<dst_state>{});
      return true;
    }
    return false;
  }
  G g;
};
template <class S2, class E, class G>
struct transition<state<internal>, state<S2>, front::event<E>, G, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = G;
  using action = none;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<G, E>>;
  transition(const G &g, const none &) : g(g) {}
  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, Ts &&...) {
    return call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs);
  }
  G g;
};
template <class S1, class S2, class E>
struct transition<state<S1>, state<S2>, front::event<E>, always, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = always;
  using action = none;
  using deps = aux::type_list<>;
  transition(const always &, const none &) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
    update_current_state(sm, deps, subs, current_state,
                         aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                         state<dst_state>{});
    sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
    return true;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    update_current_state(sm, deps, subs, current_state,
                         aux::get_id<typename SM::state_t, dst_state>((typename SM::states_ids_t *)0), state<src_state>{},
                         state<dst_state>{});
    return true;
  }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
template <class S2, class E>
struct transition<state<internal>, state<S2>, front::event<E>, always, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = always;
  using action = none;
  using deps = aux::type_list<>;
  transition(const always &, const none &) {}
  template <class TEvent, class SM, class TDeps, class TSubs, class... Ts>
  bool execute(const TEvent &, SM &, TDeps &, TSubs &, typename SM::state_t &, Ts &&...) {
    return true;
  }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
}  // namespace front
using _ = back::_;
#if !(defined(_MSC_VER) && !defined(__clang__))
template <class TEvent>
constexpr front::event<TEvent> event{};
#else
template <class TEvent>
front::event<TEvent> event __BOOST_SML_VT_INIT;
#endif
template <class TEvent>
__BOOST_SML_UNUSED front::event<back::on_entry<_, TEvent>> on_entry __BOOST_SML_VT_INIT;
template <class TEvent>
__BOOST_SML_UNUSED front::event<back::on_exit<_, TEvent>> on_exit __BOOST_SML_VT_INIT;
template <class TEvent>
front::event<back::unexpected_event<TEvent>> unexpected_event __BOOST_SML_VT_INIT;
template <class T>
front::event<back::exception<T>> exception __BOOST_SML_VT_INIT;
using anonymous = back::anonymous;
using initial = back::initial;
#if !(defined(_MSC_VER) && !defined(__clang__))
template <class T>
constexpr typename front::state_sm<T>::type state{};
#else
template <class T>
typename front::state_sm<T>::type state __BOOST_SML_VT_INIT;
#endif
inline namespace literals {
#if defined(__cpp_nontype_template_parameter_class)
template <aux::fixed_string Str>
constexpr auto operator""_s() {
  return []<auto... Ns>(aux::index_sequence<Ns...>) { return front::state<aux::string<char, Str.data[Ns]...>>{}; }
  (aux::make_index_sequence<Str.size>{});
}
template <aux::fixed_string Str>
constexpr auto operator""_e() {
  return []<auto... Ns>(aux::index_sequence<Ns...>) { return event<aux::string<char, Str.data[Ns]...>>; }
  (aux::make_index_sequence<Str.size>{});
}
#elif !(defined(_MSC_VER) && !defined(__clang__))
template <class T, T... Chrs>
constexpr auto operator""_s() {
  return front::state<aux::string<T, Chrs...>>{};
}
template <class T, T... Chrs>
constexpr auto operator""_e() {
  return event<aux::string<T, Chrs...>>;
}
#endif
}  // namespace literals
__BOOST_SML_UNUSED static front::state<back::terminate_state> X;
__BOOST_SML_UNUSED static front::history_state H;
__BOOST_SML_UNUSED static front::actions::defer defer;
__BOOST_SML_UNUSED static front::actions::process process;
__BOOST_SML_UNUSED static front::state<class SML_EVAL> eval;
template <class... Ts, __BOOST_SML_REQUIRES(aux::is_same<aux::bool_list<aux::always<Ts>::value...>,
                                                         aux::bool_list<concepts::transitional<Ts>::value...>>::value)>
auto make_transition_table(Ts... ts) {
  return aux::pool<Ts...>{ts...};
}
BOOST_SML_NAMESPACE_END
#undef __BOOST_SML_UNUSED
#undef __BOOST_SML_VT_INIT
#undef __BOOST_SML_ZERO_SIZE_ARRAY
#undef __BOOST_SML_ZERO_SIZE_ARRAY_CREATE
#undef __BOOST_SML_TEMPLATE_KEYWORD
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__) && defined(__BOOST_SML_DEFINED_HAS_BUILTIN)
#undef __has_builtin
#pragma GCC diagnostic pop
#elif defined(_MSC_VER) && !defined(__clang__) && defined(__BOOST_SML_DEFINED_HAS_BUILTIN)
#undef __has_builtin
#undef __has_builtin__make_integer_seq
#endif
#endif
#endif
