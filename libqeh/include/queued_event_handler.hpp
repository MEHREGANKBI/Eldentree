#pragma once
#include <iostream>
#include <string>

namespace qeh {

struct Event {
  std::string message{};
  Event* next{nullptr};
};

struct Queue {
  std::string name{};
  Event* first_event{nullptr};
  Event* last_event{nullptr};
  Queue* next{nullptr};

  qeh::Event* add_event_to_queue(const char* event_queue_pair);
};

struct QueueList {
  Queue* first{nullptr};
  Queue* last{nullptr};

  Queue* queue_exists(const char* event_queue_pair) const;
  qeh::Queue* create_new_queue(const char* queue_name);
};

}  // namespace qeh

template <typename T>
void abort_if_nullptr(T* ptr, const char* err_msg = "") {
  if (ptr == nullptr) {
    std::cerr << err_msg;
    std::abort();
  }
}

int32_t process_event_queue_pairs(const char* event_queue_pairs[],
                                  qeh::QueueList* queue_list);