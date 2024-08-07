#include "../include/queued_event_handler.hpp"

#include <string.h>

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// Number of times faced an empty queue.
#define TIMES_RUN_EMPTY_LIMIT 10

qeh::ErrOr<qeh::Queue> qeh::QueueList::queue_exists(
    const char* event_queue_pair) const {
  if (!event_queue_pair) {
    std::cerr << "Null event queue pair received.";
    return ErrOr<Queue>{true};
  }

  std::cout << event_queue_pair << std::endl;
  if ((this->first == nullptr) && (this->last == nullptr)) {
    return ErrOr<Queue>{false};
  }

  // Invalid state and thus not recoverable.
  // basically since the code didn't enter the && above,
  // the OR condition below acts as XOR.
  if ((this->first == nullptr) || (this->last == nullptr)) {
    std::cerr << "Only first member or only last member of queuelist is null!";
    return ErrOr<Queue>{true};
  }

  Queue* current_queue{this->first};
  while (current_queue) {
    if (current_queue->name == event_queue_pair) {
      return ErrOr<Queue>{false, current_queue};
    }
    current_queue = current_queue->next;
  }
  return ErrOr<Queue>{false};
}

qeh::ErrOr<qeh::Queue> qeh::QueueList::create_new_queue(
    const char* queue_name) {
  if (!queue_name) {
    return ErrOr<Queue>{true};
  }
  // New queue's next ptr is already null when initialized.
  qeh::Queue* new_queue{new qeh::Queue{}};
  if (!new_queue) {
    return ErrOr<Queue>{true};
  }
  new_queue->name = queue_name;

  if ((this->first == nullptr) && (this->last == nullptr)) {
    this->first = new_queue;
    this->last = new_queue;
    return ErrOr<Queue>{false, new_queue};
  }

  if ((this->first != nullptr) && (this->last != nullptr)) {
    this->last->next = new_queue;
    this->last = new_queue;
    return ErrOr<Queue>{false, new_queue};
  }

  // Invalid state: Only one member is null.
  // Unrecoverable Error
  return ErrOr<Queue>{true};
}

const char* get_event_from_pair(const char* event_queue_pair) {
  if (!event_queue_pair) {
    return nullptr;
  }
  const char* first_null_terminator_ptr{strchr(event_queue_pair, '\0')};
  if (!first_null_terminator_ptr) {
    return nullptr;
  }
  // Event section starts right after the null terminator
  const char* event_ptr{first_null_terminator_ptr + 1};
  return event_ptr;
}

qeh::ErrOr<qeh::Event> qeh::Queue::add_event_to_queue(
    const char* event_queue_pair) {
  if (!event_queue_pair) {
    std::cerr << "Null event received.";
    return ErrOr<Event>{true};
  }
  const char* event_message{get_event_from_pair(event_queue_pair)};
  if (!event_message) {
    std::cerr << "Null event message received.";
    return ErrOr<Event>{true};
  }

  std::cout << event_message << std::endl;

  qeh::Event* new_event{new qeh::Event{}};
  if (!new_event) {
    std::cerr << "Event alloc failed.";
    return ErrOr<Event>{true};
  }
  new_event->message = event_message;

  if ((this->first_event == nullptr) && (this->last_event == nullptr)) {
    this->first_event = new_event;
    this->last_event = new_event;
    return ErrOr<Event>{false, new_event};
  }

  if ((this->first_event != nullptr) && (this->last_event != nullptr)) {
    this->last_event->next = new_event;
    this->last_event = new_event;
    return ErrOr<Event>{false, new_event};
  }

  // Invalid state. Only one member is null.
  std::cerr << "Null event received.";
  return ErrOr<Event>{true};
}

int32_t process_event_queue_pairs(const char* event_queue_pairs[],
                                  qeh::QueueList* queue_list) {
  if ((!event_queue_pairs) || (!queue_list)) {
    return FATAL_ERROR;
  }

  int events_added{0};
  while (*event_queue_pairs) {
    qeh::ErrOr<qeh::Queue> queue_lookup_status{
        queue_list->queue_exists(*event_queue_pairs)};
    qeh::Queue* corresponding_queue{};
    if (queue_lookup_status.error) {
      return FATAL_ERROR;
    }
    corresponding_queue = queue_lookup_status.value;

    if (!corresponding_queue) {
      std::cout << "Not found..." << std::endl;
      qeh::ErrOr<qeh::Queue> queue_create_status{
          queue_list->create_new_queue(*event_queue_pairs)};
      if (queue_create_status.error) {
        std::cerr << "queue creation failed.";
        return FATAL_ERROR;
      }
      corresponding_queue = queue_create_status.value;
    }

    qeh::ErrOr<qeh::Event> event_create_status{
        corresponding_queue->add_event_to_queue(*event_queue_pairs)};
    if (event_create_status.error) {
      std::cerr << "Couldn't create event.";
      return FATAL_ERROR;
    }
    events_added += 1;
    // move to the next input from pair.
    event_queue_pairs += 1;
  }
  return events_added;
}

int32_t default_worker_handler(qeh::QueueList* queue_list, int32_t thread_id) {
  // Invalid and unrecoverable state.
  if ((!queue_list) || (queue_list->first == nullptr) ||
      (queue_list->last == nullptr)) {
    return EXIT_FAILURE;
  }
  static int32_t empty_queue_faced{};
  static std::mutex qeh_lock;
  static qeh::Queue* current_queue{queue_list->first};
  while (true) {
    // if all queues have been empty for a while, stop checking.
    // no lock needed.
    if (empty_queue_faced > TIMES_RUN_EMPTY_LIMIT) break;

    qeh_lock.lock();
    if (!current_queue) {
      return EXIT_SUCCESS;
    }

    // if queue has no more events.
    if ((current_queue->first_event == nullptr) ||
        (current_queue->last_event == nullptr)) {
      // move onto the next queue.
      if (current_queue->next == nullptr) {
        current_queue = queue_list->first;
      } else {
        current_queue = current_queue->next;
      }
      empty_queue_faced += 1;
      qeh_lock.unlock();
      continue;
    }

    qeh::Event* current_event{current_queue->first_event};
    std::cout << "thread " << thread_id
              << " handled: " << current_event->message << std::endl;
    // if there's only one more event remaining in the queue.
    if (current_queue->first_event == current_queue->last_event) {
      current_queue->first_event = nullptr;
      current_queue->last_event = nullptr;
    } else {
      current_queue->first_event = current_event->next;
    }
    delete current_event;

    // move to the next queue for the next cycle.
    if (current_queue->next == nullptr) {
      current_queue = queue_list->first;
    } else {
      current_queue = current_queue->next;
    }
    qeh_lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  return EXIT_SUCCESS;
}

int32_t handle_all_events(qeh::QueueList* queue_list, int32_t worker_count) {
  if ((!queue_list) || (worker_count < 1)) {
    std::cerr << "Null queue list or Invalid worker count received.";
    return EXIT_FAILURE;
  }
  std::vector<std::thread*> worker_ptrs{};
  for (int32_t id = 0; id < worker_count; ++id) {
    std::thread* new_thread{
        new std::thread{default_worker_handler, queue_list, id}};
    if (!new_thread) {
      std::cerr << "Failed to create thread.";
      return EXIT_FAILURE;
    }
    worker_ptrs.push_back(new_thread);
  }

  for (std::thread* thread_ptr : worker_ptrs) {
    thread_ptr->join();
    delete thread_ptr;
  }
  return EXIT_SUCCESS;
}