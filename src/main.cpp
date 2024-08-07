#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>

#include "queued_event_handler.hpp"

#define WORKER_COUNT 10
int main() {
  // The queue names and messages don't have to follow a pattern.
  // e.g. queue name could be "jackson" and message could be "hi".
  const char* event_queue_pairs[] = {
      "q1\0message 1 of q1, 1 in total.",
      "q2\0message 1 of q2, 2 in total.",
      "q3\0message 1 of q3, 3 in total.",
      "q1\0message 2 of q1, 4 in total.",
      "q3\0message 2 of q3, 5 in total.",
      "q1\0message 3 of q1, 6 in total.",
      "q9\0message 1 of q9, 7 in total.",
      "q1\0message 4 of q1, 8 in total",
      "q153\0message 1 of q153, 9 in total.",
      nullptr,
  };
  // it's ok to use a local variable to hold addr of queue list.
  // when this ptr gets deleted, the app will exit anyway.
  std::unique_ptr<qeh::QueueList> queue_list{new qeh::QueueList{}};

  int32_t events_processed_count{
      process_event_queue_pairs(event_queue_pairs, queue_list.get())};
  if (events_processed_count == FATAL_ERROR) {
    std::cerr << "Processing input failed." << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Events processed: " << events_processed_count << std::endl;

  std::cout << "==============" << std::endl;

  queue_list->print_queues();

  std::cout << "****************" << std::endl;

  if (handle_all_events(queue_list.get(), WORKER_COUNT) == EXIT_FAILURE) {
    std::cerr << "Event handler failed." << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}