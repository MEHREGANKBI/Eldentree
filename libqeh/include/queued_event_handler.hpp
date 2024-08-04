#include <string>

namespace qeh {

struct Event {
  std::string m_message{};
  Event* m_next{nullptr};
};

struct Queue {
  std::string m_name{};
  Event* m_first_event{nullptr};
  Event* m_last_event{nullptr};
  Queue* m_next{nullptr};
};

struct QueueList {
  Queue* m_first{nullptr};
  Queue* m_last{nullptr};
};

}  // namespace qeh