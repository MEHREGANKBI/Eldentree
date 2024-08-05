#include "../include/queued_event_handler.hpp"

void qeh::Queue::print_events() const {
  Event* current_event{this->first_event};
  while (current_event) {
    std::cout << current_event->message << '\n';
    current_event = current_event->next;
  }
}

void qeh::QueueList::print_queues() const {
  Queue* current_queue{this->first};
  while (current_queue) {
    std::cout << current_queue->name << '\n';
    current_queue->print_events();
    std::cout << "========" << std::endl;
    current_queue = current_queue->next;
  }
}