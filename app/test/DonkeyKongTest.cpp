#include <gtest/gtest.h>
#include "model/DonkeyKong.hpp"

TEST(DonkeyKongTest, ConstructorSetsPosition) {
    DonkeyKong dk(60.f, 140.f, 1);
    auto pos = dk.get_shape().getPosition();
    EXPECT_NEAR(pos.x, 60.f, 1.f);
    EXPECT_NEAR(pos.y, 140.f, 1.f);
}

TEST(DonkeyKongTest, GetBoundsNonEmpty) {
    DonkeyKong dk(60.f, 140.f, 1);
    auto b = dk.get_bounds();
    EXPECT_GT(b.size.x, 0);
    EXPECT_GT(b.size.y, 0);
}
