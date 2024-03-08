#ifndef STATIC_DEQUE_H
#define STATIC_DEQUE_H

#include <array>
#include <stdexcept>

template <typename T, std::size_t Size>
struct MinimalStaticDeque {
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

  void push_back(T&& t) { queue_data[current_index++] = std::move(t); }
  T& front() {
    if (current_index == 0) {
      throw std::out_of_range("queue is empty");
    }
    return queue_data[0];
  }
  T& back() {
    if (current_index == 0) {
      throw std::out_of_range("queue is empty");
    }
    return queue_data[current_index - 1];
  }
  void pop_front() {
    if (current_index == 0) {
      throw std::out_of_range("queue is empty");
    }
    std::move(std::next(queue_data.begin()), queue_data.end(), queue_data.begin());
    current_index--;
  }
  void pop_back() {  // removes the last element
    if (current_index == 0) {
      throw std::out_of_range("queue is empty");
    }
    current_index--;
  }
  void push_front(T&& value) {
    if (current_index == Size) {
      throw std::runtime_error("queue is full");
    }
    std::move_backward(begin(), end(), std::next(end()));
    queue_data[0] = std::move(value);
    current_index++;
  }
  bool empty() const { return current_index == 0; }

  iterator begin() { return queue_data.begin(); }
  const_iterator cbegin() const { return queue_data.cbegin(); }

  iterator end() { return std::next(queue_data.begin(), current_index); }
  const_iterator cend() const { return std::next(queue_data.begin(), current_index); }

  iterator erase(const_iterator pos) {
    if (pos == end()) {
      throw std::out_of_range("queue is empty");
    }
    const auto position = std::distance(cbegin(), pos);
    auto start = queue_data.begin() + position;
    std::move(std::next(start), end(), start);
    current_index--;
    return start;
  }
};

#endif  // STATIC_DEQUE_H
