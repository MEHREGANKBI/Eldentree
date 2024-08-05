#include <cstdlib>
#include <memory>

#include "../libqeh/include/queued_event_handler.hpp"

#define WORKER_COUNT 10
int main() {
  const char* event_queue_pairs[] = {
      "q1\0hi there!",
      "q2\0hello!",
      "q3\0ok!",
      "q2\0no!",
      "q1\0yes!",
      "q1\0alright!",
      "gooood\0this is a longer message.",
      "q1\0this is a test.",
      nullptr,
  };

  std::unique_ptr<qeh::QueueList> queue_list{new qeh::QueueList{}};
  abort_if_nullptr(queue_list.get(), "queue list alloc failed. Aborting...");

  std::cout << process_event_queue_pairs(event_queue_pairs, queue_list.get())
            << std::endl;

  queue_list->print_queues();

  std::cout << "****************" << std::endl;

  handle_all_events(queue_list.get(), WORKER_COUNT);

  return EXIT_SUCCESS;
}