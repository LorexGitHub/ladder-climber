#include <gtest/gtest.h>
#include "model/PowerUp.hpp"

TEST(PowerUpTest, StartsActive) {
    PowerUp p(100, 200);
    EXPECT_TRUE(p.is_active());
}

TEST(PowerUpTest, CollectDeactivates) {
    PowerUp p(100, 200);
    p.collect();
    EXPECT_FALSE(p.is_active());
}

TEST(PowerUpTest, StaysActiveAfterUpdate) {
    PowerUp p(100, 200);
    p.update(10.f);
    EXPECT_TRUE(p.is_active());
}

TEST(PowerUpTest, GetBoundsReturnsArea) {
    PowerUp p(100, 200);
    auto b = p.get_bounds();
    EXPECT_GT(b.size.x, 0);
    EXPECT_GT(b.size.y, 0);
}
