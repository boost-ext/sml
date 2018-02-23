#include "boost/sml/back/policies.hpp"
#include "boost/sml/front/actions/defer.hpp"

template <class T>
struct thread_safe : aux::pair<back::thread_safety_policy__, thread_safe<T>> {
  using type = T;
};

template <template <class...> class T>
struct defer_queue : aux::pair<back::defer_queue_policy__, defer_queue<T>> {
  template <class U>
  using rebind = T<U>;

  template <class... Ts>
  using defer = front::actions::defer_event<Ts...>;
};

template <class T>
struct logger : aux::pair<back::logger_policy__, logger<T>> {
  using type = T;
};

struct testing : aux::pair<back::testing_policy__, testing> {};
