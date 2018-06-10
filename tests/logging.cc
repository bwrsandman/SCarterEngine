//
// Created by sandy on 12/05/18.
//

#include <Logging.h>
#include <Scripting.h>
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

TEST_F(LoggingTest, scripting) {
  std::string output, expected_output;
  const auto source =
      std::string(R"(Engine.Logging.Log("Debug", "Test Message"))");

  sce::scripting::Initialize();

  testing::internal::CaptureStdout();
  ASSERT_EQ(sce::scripting::LoadSource(source), EXIT_SUCCESS);
  output = testing::internal::GetCapturedStdout();
  expected_output =
      std::string("DEBUG: ") +
      "[string \"Engine.Logging.Log(\"Debug\", \"Test Message\")\"]:1" +
      ": Test Message\n";
  EXPECT_STREQ(output.c_str(), expected_output.c_str());

  sce::scripting::Terminate();
}

TEST_F(LoggingTest, scripting_bad_enum) {
  const auto source =
      std::string(R"(Engine.Logging.Log("BadEnum", "Test Message"))");
  sce::scripting::Initialize();
  ASSERT_DEATH(sce::scripting::LoadSource(source), "assert true failed");
  sce::scripting::Terminate();
}
