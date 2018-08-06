//
// Created by sandy on 05/08/18.
//

#include <gtest/gtest.h>
#include "../src/private/Command/ReusableCommandQueue.h"

TEST(command_test, reusable_command_queue) {
  class TestCommand1 : public sce::command::Command {
   public:
    void Execute() const override {
      std::cout << "Executing!" << std::endl;
    }
  };

  class TestCommand2 : public sce::command::Command {
    const std::string message_;

   public:
    explicit TestCommand2(const std::string && message) : message_(message) {}
    void Execute() const override {
      std::cout << message_ << std::endl;
    }
  };

  class TestCommand3 : public sce::command::Command {
   public:
    explicit TestCommand3(bool * pTestOk) : pTestOk(pTestOk) {}
    bool * const pTestOk;

    void Execute() const override {
      *pTestOk = true;
    }
  };

  sce::command::ReusableCommandQueue commandQueue;
  ASSERT_NO_FATAL_FAILURE(commandQueue.Execute());

  commandQueue.Emplace(new TestCommand1());
  testing::internal::CaptureStdout();
  commandQueue.Execute();
  EXPECT_STREQ(testing::internal::GetCapturedStdout().c_str(), "Executing!\n");

  commandQueue.Emplace(new TestCommand2("Executing 2"));
  commandQueue.Emplace(new TestCommand2("Executing 3"));
  testing::internal::CaptureStdout();
  commandQueue.Execute();
  EXPECT_STREQ(testing::internal::GetCapturedStdout().c_str(),
               "Executing!\nExecuting 2\nExecuting 3\n");

  bool testOK = false;
  commandQueue.Emplace(new TestCommand3(&testOK));
  ASSERT_FALSE(testOK);
  testing::internal::CaptureStdout();
  commandQueue.Execute();
  EXPECT_STREQ(testing::internal::GetCapturedStdout().c_str(),
               "Executing!\nExecuting 2\nExecuting 3\n");
  ASSERT_TRUE(testOK);
}
