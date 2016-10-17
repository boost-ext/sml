#ifndef QUEUE_Q3QVBZJG
#define QUEUE_Q3QVBZJG

#include "boost/msm-lite/front/fwd.hpp"

namespace detail {

struct queue {
  template <class TEvent>
  class queue_impl : public action_base {
   public:
    explicit queue_impl(const TEvent &event) : event(event) {}

    template <class TSelf, class T>
    void operator()(TSelf &self, const T &) {
      self.me_.process_event(event, self.deps_, self.sub_sms_);
    }

   private:
    TEvent event;
  };

  template <class TEvent>
  auto operator()(const TEvent &event) {
    return queue_impl<TEvent>{event};
  }
};

}  // detail

#endif
