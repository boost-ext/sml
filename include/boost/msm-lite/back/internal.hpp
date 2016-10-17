#ifndef INTERNAL_JJDYJJ83
#define INTERNAL_JJDYJJ83

namespace detail {

struct internal_event {
  static auto c_str() { return "internal_event"; }
};
struct anonymous : internal_event {
  static auto c_str() { return "anonymous"; }
};
struct on_entry : internal_event {
  static auto c_str() { return "on_entry"; }
};
struct on_exit : internal_event {
  static auto c_str() { return "on_exit"; }
};

}  // detail
