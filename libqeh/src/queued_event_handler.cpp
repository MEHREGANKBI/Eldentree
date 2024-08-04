#include "../include/queued_event_handler.hpp"

#include <iostream>

qeh::Queue* qeh::QueueList::queue_exists(const char* event_queue_pair) const {
  std::cout << event_queue_pair << std::endl;
  if ((this->first == nullptr) && (this->last == nullptr)) {
    return nullptr;
  }

  // Invalid state and thus should lead to abort.
  if ((this->first == nullptr) || (this->last == nullptr)) {
    std::cerr << "Only first member or only last member of queuelist is null! "
                 "Aborting...";
    std::abort();
  }

  Queue* current_queue{this->first};
  while (current_queue) {
    if (current_queue->name == event_queue_pair) {
      return current_queue;
    }

    current_queue = current_queue->next;
  }

  return nullptr;
}

int32_t process_event_queue_pairs(const char* event_queue_pairs[],
                                  qeh::QueueList* queue_list) {
  int events_added{0};
  return events_added;
}