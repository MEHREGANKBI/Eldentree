#include "../include/queued_event_handler.hpp"

#include <string.h>

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#define TIMES_RUN_EMPTY_LIMIT 10

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

qeh::Queue* qeh::QueueList::create_new_queue(const char* queue_name) {
  // New queue's next ptr is already null when initialized.
  qeh::Queue* new_queue{new qeh::Queue{}};
  abort_if_nullptr(new_queue, "Failed to alloc new queue. Aborting...");
  new_queue->name = queue_name;

  if ((this->first == nullptr) && (this->last == nullptr)) {
    this->first = new_queue;
    this->last = new_queue;
    return new_queue;
  }

  if ((this->first != nullptr) && (this->last != nullptr)) {
    this->last->next = new_queue;
    this->last = new_queue;
    return new_queue;
  }

  // Invalid state: Only one member is null.
  std::cerr << "Only first member or only last member of queuelist is null! "
               "Aborting...";
  std::abort();
}

const char* get_event_from_pair(const char* event_queue_pair) {
  const char* first_null_terminator{strchr(event_queue_pair, '\0')};
  abort_if_nullptr(first_null_terminator, "Parsing event failed. Aborting...");
  const char* event{first_null_terminator + 1};
  return event;
}

qeh::Event* qeh::Queue::add_event_to_queue(const char* event_queue_pair) {
  const char* event_message{get_event_from_pair(event_queue_pair)};
  abort_if_nullptr(event_message, "event message is null");
  std::cout << event_message << std::endl;

  qeh::Event* new_event{new qeh::Event{}};
  abort_if_nullptr(new_event, "Failed to alloc event. Aborting...");
  new_event->message = event_message;

  if ((this->first_event == nullptr) && (this->last_event == nullptr)) {
    this->first_event = new_event;
    this->last_event = new_event;
    return new_event;
  }

  if ((this->first_event != nullptr) && (this->last_event != nullptr)) {
    this->last_event->next = new_event;
    this->last_event = new_event;
    return new_event;
  }

  // Invalid state. Only one member is null.
  std::cerr << "Only first or last event is null. Aborting...";
  std::abort();
}

int32_t process_event_queue_pairs(const char* event_queue_pairs[],
                                  qeh::QueueList* queue_list) {
  abort_if_nullptr(queue_list, "Received null as queue list ptr.");
  abort_if_nullptr(event_queue_pairs, "Received null as input ptr.");
  int events_added{0};
  while (*event_queue_pairs) {
    qeh::Queue* corresponding_queue{
        queue_list->queue_exists(*event_queue_pairs)};

    if (!corresponding_queue) {
      std::cout << "Not found..." << std::endl;
      corresponding_queue = queue_list->create_new_queue(*event_queue_pairs);
      abort_if_nullptr(corresponding_queue, "Failed to alloc new queue.");
    }

    qeh::Event* event_created{
        corresponding_queue->add_event_to_queue(*event_queue_pairs)};
    abort_if_nullptr(corresponding_queue, "Failed to alloc new queue.");
    // move to the next input from pair.
    event_queue_pairs += 1;
    events_added += 1;
  }
  return events_added;
}

void default_worker_handler(qeh::QueueList* queue_list, int32_t thread_id) {
  // Invalid state.
  if ((queue_list->first == nullptr) || (queue_list->last == nullptr)) {
    std::abort();
  }
  static int32_t empty_queue_faced{};
  static std::mutex qeh_lock;
  static qeh::Queue* current_queue{queue_list->first};
  qeh::Event* current_event{nullptr};
  while (true) {
    // if all queues have been empty for a while, stop checking.
    // no lock needed.
    if (empty_queue_faced > TIMES_RUN_EMPTY_LIMIT) break;

    qeh_lock.lock();
    abort_if_nullptr(current_queue, "null queue ptr faced.");

    // if queue has no more events.
    if ((current_queue->first_event == nullptr) ||
        (current_queue->last_event == nullptr)) {
      // skip current queue.
      if (current_queue->next == nullptr) {
        current_queue = queue_list->first;
      } else {
        current_queue = current_queue->next;
      }
      empty_queue_faced += 1;
      qeh_lock.unlock();
      continue;
    }

    current_event = current_queue->first_event;
    std::cout << "thread " << thread_id
              << " handled: " << current_event->message << std::endl;
    // if there's only one more event remaining.
    if (current_queue->first_event == current_queue->last_event) {
      current_queue->first_event = nullptr;
      current_queue->last_event = nullptr;
    } else {
      current_queue->first_event = current_event->next;
    }
    delete current_event;

    // move to the next queue
    if (current_queue->next == nullptr) {
      current_queue = queue_list->first;
    } else {
      current_queue = current_queue->next;
    }
    qeh_lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
}

void handle_all_events(qeh::QueueList* queue_list, int32_t worker_count) {
  std::vector<std::thread*> worker_ptrs{};
  for (int32_t id = 0; id < worker_count; ++id) {
    std::thread* new_thread{
        new std::thread{default_worker_handler, queue_list, id}};
    abort_if_nullptr(new_thread, "Failed to create a thread.");
    worker_ptrs.push_back(new_thread);
  }

  for (std::thread* thread_ptr : worker_ptrs) {
    thread_ptr->join();
    delete thread_ptr;
  }
}