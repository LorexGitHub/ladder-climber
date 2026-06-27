#include <gtest/gtest.h>
#include "model/Barrel.hpp"

TEST(BarrelTest, DefaultAlive) {
    Barrel b(100, 200, 180, 0);
    EXPECT_TRUE(b.is_alive());
}

TEST(BarrelTest, MovesHorizontally) {
    Barrel b(100, 200, 180, 0);
    float x0 = b.get_pos().x;
    b.update(0.1f);
    EXPECT_GT(b.get_pos().x, x0);
}

TEST(BarrelTest, LandingOnSurface) {
    Barrel b(100, 200, 180, 0);
    float surface_y = 300.f;
    b.land_on_surface(surface_y);
    EXPECT_EQ(b.get_pos().y, surface_y);
}

TEST(BarrelTest, KillMarksDead) {
    Barrel b(100, 200, 180, 0);
    b.kill();
    EXPECT_FALSE(b.is_alive());
}

TEST(BarrelTest, GravityApplies) {
    Barrel b(100, 200, 0, 0);
    b.update(0.1f);
    EXPECT_GT(b.get_pos().y, 200);
}

TEST(BarrelTest, OutOfBoundsKills) {
    Barrel b(100, 800, 0, 0);
    EXPECT_TRUE(b.is_alive());
    b.update(0.01f);
    EXPECT_FALSE(b.is_alive());
}
