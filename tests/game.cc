//
// Created by sandy on 31/05/18.
//

#include <Game.h>
#include <Scripting.h>
#include <gtest/gtest.h>

class GameTest : public ::testing::Test {
 protected:
  void SetUp() override {
    sce::game::Initialize();
  }

  void TearDown() override {
    sce::game::Terminate();
  }
};

TEST(scene_test, scene_init_terminate) {
  ASSERT_NO_FATAL_FAILURE(sce::game::Initialize());
  ASSERT_NO_FATAL_FAILURE(sce::game::Terminate());
  ASSERT_NO_FATAL_FAILURE(sce::game::Initialize());
  ASSERT_NO_FATAL_FAILURE(sce::game::Terminate());
  ASSERT_NO_FATAL_FAILURE(sce::game::Initialize());
  ASSERT_NO_FATAL_FAILURE(sce::game::Terminate());
}

TEST_F(GameTest, Scenes) {
  // Check duplicate inserts and missing removals
  ASSERT_NO_FATAL_FAILURE(sce::game::AddScene("Test"));
  ASSERT_DEATH(sce::game::AddScene("Test"), "assert true failed");
  ASSERT_NO_FATAL_FAILURE(sce::game::RemoveScene("Test"));
  ASSERT_DEATH(sce::game::RemoveScene("Test"), "assert false failed");

  // Check clearing at terminate
  ASSERT_NO_FATAL_FAILURE(sce::game::AddScene("Test"));
  sce::game::Terminate();
  sce::game::Initialize();
  ASSERT_NO_FATAL_FAILURE(sce::game::AddScene("Test"));
  sce::game::Terminate();
  ASSERT_DEATH(sce::game::RemoveScene("Test"), "assert false failed");
  sce::game::Initialize();
}

TEST_F(GameTest, scripting) {
  const auto source = std::string(
      "Engine.game.AddScene(\"Test\")\n"
      "Engine.game.RemoveScene(\"Test\")");

  sce::scripting::Initialize();

  ASSERT_EQ(sce::scripting::RunSource(source), EXIT_SUCCESS);

  sce::scripting::Terminate();
}
