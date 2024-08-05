#include <cstdlib>

#include "../libqeh/include/queued_event_handler.hpp"

int main() {
  const char* event_queue_pairs[] = {
      "q1\0hi there!",
      "q2\0hello!",
      "q3\0ok!",
      "q2\0no!",
      "q1\0yes!",
      "q1\0alright!",
      "gooood\0this is a longer message.",
      nullptr,
  };

  qeh::QueueList* queue_list{new qeh::QueueList{}};
  abort_if_nullptr(queue_list, "queue list alloc failed. Aborting...");

  std::cout << process_event_queue_pairs(event_queue_pairs, queue_list)
            << std::endl;

  return EXIT_SUCCESS;
}