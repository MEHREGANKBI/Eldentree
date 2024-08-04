#include <string>
#include <string_view>

namespace qeh {

class Event {
 private:
  std::string m_message{};
  Event* m_next{nullptr};
};

class Queue {
 private:
  std::string m_name{};
  Event* m_first_event{nullptr};
  Event* m_last_event{nullptr};
  Queue* m_next{nullptr};
};

class QueueList {
 private:
  Queue* m_first{nullptr};
  Queue* m_last{nullptr};
};

}  // namespace qeh