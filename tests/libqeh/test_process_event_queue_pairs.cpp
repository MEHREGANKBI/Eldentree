#include <gtest/gtest.h>

#include <cstdint>

#include "queued_event_handler.hpp"

TEST(test_process_event_queue_pairs, test_empty_input) {
  qeh::QueueList queue_list{};
  const char* event_queue_pairs[] = {nullptr, nullptr};
  int32_t expected{0};
  EXPECT_EQ(expected,
            process_event_queue_pairs(event_queue_pairs, &queue_list));
}

TEST(test_process_event_queue_pairs, test_2_inputs) {
  qeh::QueueList queue_list{};
  const char* event_queue_pairs[] = {"q1\0this is q1.", "q2\0this is q2.",
                                     nullptr};
  int32_t expected{2};
  EXPECT_EQ(expected,
            process_event_queue_pairs(event_queue_pairs, &queue_list));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}