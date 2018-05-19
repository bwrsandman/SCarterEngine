//
// Created by sandy on 11/05/18.
//

#include <Debug.h>
#include <Scripting.h>
#include <gtest/gtest.h>

TEST(assert_test, assert_test_true) {
  ASSERT_NO_FATAL_FAILURE(DEBUG_RUNTIME_ASSERT_TRUE(true));
  int line = __LINE__ + 1;
  ASSERT_DEBUG_DEATH(DEBUG_RUNTIME_ASSERT_TRUE(false),
                     std::string(FILE_BASENAME) + ":" + std::to_string(line) +
                         ": assert true failed");
}

TEST(assert_test, assert_test_notnull) {
  ASSERT_NO_FATAL_FAILURE(
      DEBUG_RUNTIME_ASSERT_NOT_NULL(reinterpret_cast<void *>(1)));
  int line = __LINE__ + 1;
  ASSERT_DEBUG_DEATH(DEBUG_RUNTIME_ASSERT_NOT_NULL(nullptr),
                     std::string(FILE_BASENAME) + ":" + std::to_string(line) +
                         ": pointer is null");
}

TEST(assert_test, scripting) {
  const auto source = std::string(
      "Engine.Debug.AssertTrue(true)\n"
      "Engine.Debug.AssertFalse(false)\n"
      "Engine.Debug.AssertNotNull(1)\n");
  sce::scripting::Initialize();
  ASSERT_EQ(sce::scripting::RunSource(source), EXIT_SUCCESS);
  sce::scripting::Terminate();
}
