//
// Created by sandy on 08/06/18.
//

#include <Rendering.h>
#include <gtest/gtest.h>

class RenderingTest : public ::testing::Test {
 protected:
  void SetUp() override {
    sce::rendering::Initialize();
  }
  void TearDown() override {
    sce::rendering::Terminate();
  }
};

TEST(rendering_test, rendering_init_terminate) {
  ASSERT_NO_FATAL_FAILURE(sce::rendering::Initialize());
  ASSERT_NO_FATAL_FAILURE(sce::rendering::Terminate());
  ASSERT_NO_FATAL_FAILURE(sce::rendering::Initialize());
  ASSERT_NO_FATAL_FAILURE(sce::rendering::Terminate());
  ASSERT_NO_FATAL_FAILURE(sce::rendering::Initialize());
  ASSERT_NO_FATAL_FAILURE(sce::rendering::Terminate());
}
