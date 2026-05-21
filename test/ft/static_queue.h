#ifndef STATIC_QUEUE_H
#define STATIC_QUEUE_H

#include <array>
#include <cassert>

template <typename T, std::size_t Size>
struct MinimalStaticQueue {
  std::array<T, Size> queue_data{};
  std::size_t current_index = 0;

  using value_type = typename decltype(queue_data)::value_type;
  using reference = typename decltype(queue_data)::reference;
  using const_reference = typename decltype(queue_data)::const_reference;
  using size_type = typename decltype(queue_data)::size_type;
  using difference_type = typename decltype(queue_data)::difference_type;
  using pointer = typename decltype(queue_data)::pointer;
  using const_pointer = typename decltype(queue_data)::const_pointer;
  using iterator = typename decltype(queue_data)::iterator;
  using const_iterator = typename decltype(queue_data)::const_iterator;
  using reverse_iterator = typename decltype(queue_data)::reverse_iterator;
  using const_reverse_iterator = typename decltype(queue_data)::const_reverse_iterator;

  void push(T&& t) { queue_data[current_index++] = std::move(t); }
  T& front() {
    assert(current_index != 0 && "queue is empty");
    return queue_data[0];
  }
  T& back() {
    assert(current_index != 0 && "queue is empty");
    return queue_data[current_index - 1];
  }
  iterator begin() { return queue_data.begin(); }
  iterator end() { return std::next(queue_data.begin(), current_index); }

  void pop() {
    assert(current_index != 0 && "queue is empty");
    std::move(std::next(begin()), end(), begin());
    current_index--;
  }
  bool empty() const { return current_index == 0; }
};

#endif  // STATIC_QUEUE_H
