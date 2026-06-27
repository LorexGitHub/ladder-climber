#include <gtest/gtest.h>
#include "model/Coin.hpp"

TEST(CoinTest, StartsAlive) {
    Coin c(100, 200);
    EXPECT_TRUE(c.is_alive());
}

TEST(CoinTest, CollectKillsCoin) {
    Coin c(100, 200);
    c.collect();
    EXPECT_FALSE(c.is_alive());
}

TEST(CoinTest, BoundsAtPosition) {
    Coin c(100, 200);
    auto b = c.get_bounds();
    EXPECT_GT(b.size.x, 0);
    EXPECT_GT(b.size.y, 0);
    EXPECT_NEAR(b.position.x + b.size.x / 2, 100 + 12, 7);
}

TEST(CoinTest, MultipleCoinsIndependent) {
    Coin a(100, 200);
    Coin b(300, 200);
    a.collect();
    EXPECT_FALSE(a.is_alive());
    EXPECT_TRUE(b.is_alive());
}
