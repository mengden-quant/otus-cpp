#include <gtest/gtest.h>

#include "lib.h"

TEST(BuildTest, CheckVersionAboveZero) {
	EXPECT_GT(version(), 0);
}
