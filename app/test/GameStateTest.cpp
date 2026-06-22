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
