#include <gtest/gtest.h>
#include "model/GameState.hpp"

TEST(GameStateTest, FmtTimeNegative) {
    EXPECT_EQ(GameState::fmt_time(-1), "NONE");
}

TEST(GameStateTest, FmtTimeZero) {
    EXPECT_EQ(GameState::fmt_time(0), "00:00.00");
}

TEST(GameStateTest, FmtTimeNormal) {
    EXPECT_EQ(GameState::fmt_time(65.5), "01:05.50");
}

TEST(GameStateTest, EasySpeed) {
    GameState gs;
    gs.difficulty = GameState::Difficulty::Easy;
    EXPECT_EQ(gs.get_barrel_speed(), 120.f);
}

TEST(GameStateTest, HardSpeed) {
    GameState gs;
    gs.difficulty = GameState::Difficulty::Hard;
    EXPECT_EQ(gs.get_barrel_speed(), 260.f);
}

TEST(GameStateTest, CustomSpeed) {
    GameState gs;
    gs.difficulty = GameState::Difficulty::Custom;
    gs.custom_speed = 200.f;
    EXPECT_EQ(gs.get_barrel_speed(), 200.f);
}

TEST(GameStateTest, CustomInterval) {
    GameState gs;
    gs.difficulty = GameState::Difficulty::Custom;
    gs.custom_interval = 3.5f;
    EXPECT_EQ(gs.get_barrel_interval(), 3.5f);
}
