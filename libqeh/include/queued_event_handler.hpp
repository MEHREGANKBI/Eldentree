#pragma once
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
};

struct QueueList {
  Queue* first{nullptr};
  Queue* last{nullptr};
  Queue* queue_exists(const char* event_queue_pair) const;
};

}  // namespace qeh

int32_t process_event_queue_pairs(const char* event_queue_pairs[],
                                  qeh::QueueList* queue_list);