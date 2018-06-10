//
// Created by sandy on 18/05/18.
//

#include <Logging.h>
#include <Scripting.h>
#include <gtest/gtest.h>
#include <cstdlib>

class ScriptingTest : public ::testing::Test {
 protected:
  void SetUp() override {
    sce::scripting::Initialize();
  }

  void TearDown() override {
    sce::scripting::Terminate();
  }
};

TEST(scripting_test, scripting_init_terminate) {
  ASSERT_NO_FATAL_FAILURE(sce::scripting::Initialize());
  ASSERT_NO_FATAL_FAILURE(sce::scripting::Terminate());
  ASSERT_NO_FATAL_FAILURE(sce::scripting::Initialize());
  ASSERT_NO_FATAL_FAILURE(sce::scripting::Terminate());
  ASSERT_NO_FATAL_FAILURE(sce::scripting::Initialize());
  ASSERT_NO_FATAL_FAILURE(sce::scripting::Terminate());
}

TEST_F(ScriptingTest, run_string_print) {
  testing::internal::CaptureStdout();
  ASSERT_EQ(sce::scripting::LoadSource("print(\"Test\")\n"), EXIT_SUCCESS);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_STREQ(output.c_str(), "Test\n");
}

TEST_F(ScriptingTest, run_string_error_syntax) {
  sce::logging::Initialize();
  testing::internal::CaptureStdout();
  ASSERT_EQ(sce::scripting::LoadSource("print("), EXIT_FAILURE);
  std::string output;
  output = testing::internal::GetCapturedStdout();
  EXPECT_PRED_FORMAT2(::testing::IsSubstring,
                      "[string \"print(\"]:1: unexpected symbol near <eof>",
                      output.c_str());
  sce::logging::Terminate();
}

TEST_F(ScriptingTest, run_string) {
  testing::internal::CaptureStdout();
  auto source =
      "function max(num1, num2)\n"
      "\n"
      "    if (num1 > num2) then\n"
      "        result = num1;\n"
      "    else\n"
      "        result = num2;\n"
      "    end\n"
      "\n"
      "    return result;\n"
      "end\n"
      "\n"
      "-- calling a function\n"
      "print(max(10, 4))\n"
      "print(max(5, 6))";
  ASSERT_EQ(sce::scripting::LoadSource(source), EXIT_SUCCESS);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_STREQ(output.c_str(), "10\n6\n");
}

TEST_F(ScriptingTest, run_file_error_syntax) {
  sce::logging::Initialize();
  auto filename = test_info_->test_case_name() + std::string(".") +
                  test_info_->name() + ".lua";
  testing::internal::CaptureStdout();
  ASSERT_EQ(sce::scripting::LoadFile(filename), EXIT_FAILURE);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_PRED_FORMAT2(::testing::IsSubstring, "unexpected symbol near <eof>",
                      output.c_str());
  sce::logging::Terminate();
}

TEST_F(ScriptingTest, run_file) {
  auto filename = test_info_->test_case_name() + std::string(".") +
                  test_info_->name() + ".lua";
  testing::internal::CaptureStdout();
  ASSERT_EQ(sce::scripting::LoadFile(filename), EXIT_SUCCESS);
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_STREQ(output.c_str(), "10\n6\n");
}
