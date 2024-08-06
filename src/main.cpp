#include <cstdlib>
#include <memory>

#include "../libqeh/include/queued_event_handler.hpp"

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

  std::unique_ptr<qeh::QueueList> queue_list{new qeh::QueueList{}};
  abort_if_nullptr(queue_list.get(), "queue list alloc failed. Aborting...");

  std::cout << process_event_queue_pairs(event_queue_pairs, queue_list.get())
            << std::endl;

  std::cout << "==============" << std::endl;
  queue_list->print_queues();

  std::cout << "****************" << std::endl;

  handle_all_events(queue_list.get(), WORKER_COUNT);

  return EXIT_SUCCESS;
}