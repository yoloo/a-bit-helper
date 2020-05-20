//
// Created by zhaoyongle on 2020/5/20.
//

#include "leetcode.h"
#include "gtest/gtest.h"

// twoSum1
TEST(ArrayTestCase, twoSum1_01)
{
    std::vector<int> target{0, 1};
    std::vector<int> result = LeetCode::Array::twoSum_1({2, 7, 11, 15}, 9);

    for (int i = 0; i < result.size(); ++i) {
        EXPECT_EQ(target[i], result[i]);
    }
}

TEST(ArrayTestCase, twoSum1_02)
{
    std::vector<int> target{0, 1};
    std::vector<int> result = LeetCode::Array::twoSum_1({2, 7, 11, 15}, 20);

    for (int i = 0; i < result.size(); ++i) {
        EXPECT_EQ(target[i], result[i]);
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}