#include <gtest/gtest.h>
#include "model/Player.hpp"

TEST(PlayerTest, JumpSetsVelocity) {
    Player p;
    p.set_position(100, 200);
    p.jump();
    EXPECT_FALSE(p.is_on_ground());
    float y0 = p.get_pos().y;
    p.update(0.016f);
    EXPECT_LT(p.get_pos().y, y0);
}

TEST(PlayerTest, ClimbingStopsHorizontal) {
    Player p;
    p.set_position(100, 200);
    p.move_right();
    p.climb(100.f);
    EXPECT_TRUE(p.is_climbing());
    float x0 = p.get_pos().x;
    p.update(0.016f);
    EXPECT_EQ(p.get_pos().x, x0);
}

TEST(PlayerTest, FallsDownDueToGravity) {
    Player p;
    p.set_position(100, 200);
    // Player starts on ground from set_position; push off to start falling
    p.jump();
    // Run many frames to go past apex and return to ground
    for (int i = 0; i < 200; i++)
        p.update(0.016f);
    // Player should be back on ground
    EXPECT_TRUE(p.is_on_ground());
}
