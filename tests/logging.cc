//
// Created by sandy on 12/05/18.
//

#include <Logging.h>
#include <gtest/gtest.h>

class LoggingTest : public ::testing::Test {
 protected:
  void SetUp() override {
    sce::logging::Initialize();
  }

  void TearDown() override {
    sce::logging::Terminate();
  }
};

TEST(logging_test, logging_init_terminate) {
  ASSERT_NO_FATAL_FAILURE(sce::logging::Initialize());
  ASSERT_NO_FATAL_FAILURE(sce::logging::Terminate());
  ASSERT_NO_FATAL_FAILURE(sce::logging::Initialize());
  ASSERT_NO_FATAL_FAILURE(sce::logging::Terminate());
  ASSERT_NO_FATAL_FAILURE(sce::logging::Initialize());
  ASSERT_NO_FATAL_FAILURE(sce::logging::Terminate());
}

TEST_F(LoggingTest, logging_test_simple) {
  testing::internal::CaptureStdout();
  int line = __LINE__ + 1;
  LOG(sce::logging::Level::Debug, "Test Message");
  std::string output = testing::internal::GetCapturedStdout();
  auto expected_output = std::string("DEBUG: ") + FILE_BASENAME + ":" +
                         std::to_string(line) + ": Test Message\n";
  EXPECT_STREQ(output.c_str(), expected_output.c_str());
}
