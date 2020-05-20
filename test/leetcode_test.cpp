//
// Created by zhaoyongle on 2020/5/20.
//

#include "leetcode/leetcode.h"
#include "gtest/gtest.h"

// 1. 两数之和

auto verifyTwoSum = [](const std::vector<int>& expect, const std::vector<int>& actual) -> bool {
    if (expect.size() != actual.size())
        return false;

    for (int i = 0; i < actual.size(); ++i) {
        if (expect[i] != actual[i])
            return false;
    }

    return true;
};

TEST(ArrayTestCase, TwoSum_Find)
{
    ASSERT_TRUE(verifyTwoSum({0, 1}, LeetCode::Array::TwoSum::one({2, 7, 11, 15}, 9)));
    ASSERT_TRUE(verifyTwoSum({1, 2}, LeetCode::Array::TwoSum::one({2, 7, -2, 15}, 5)));
    ASSERT_TRUE(verifyTwoSum({1, 2}, LeetCode::Array::TwoSum::one({2, 7, -8, 15}, -1)));
    ASSERT_TRUE(verifyTwoSum({2, 3}, LeetCode::Array::TwoSum::one({2, 7, -11, -15}, -26)));

    ASSERT_TRUE(verifyTwoSum({0, 1}, LeetCode::Array::TwoSum::two({2, 7, 11, 15}, 9)));
    ASSERT_TRUE(verifyTwoSum({1, 2}, LeetCode::Array::TwoSum::two({2, 7, -2, 15}, 5)));
    ASSERT_TRUE(verifyTwoSum({1, 2}, LeetCode::Array::TwoSum::two({2, 7, -8, 15}, -1)));
    ASSERT_TRUE(verifyTwoSum({2, 3}, LeetCode::Array::TwoSum::two({2, 7, -11, -15}, -26)));
}

TEST(ArrayTestCase, TwoSum_NotFind)
{
    EXPECT_TRUE(verifyTwoSum({}, LeetCode::Array::TwoSum::one({}, 6)));
    EXPECT_TRUE(verifyTwoSum({}, LeetCode::Array::TwoSum::one({6}, 6)));
    EXPECT_TRUE(verifyTwoSum({}, LeetCode::Array::TwoSum::one({2, 7, 11, 15}, 6)));

    EXPECT_TRUE(verifyTwoSum({}, LeetCode::Array::TwoSum::two({}, 6)));
    EXPECT_TRUE(verifyTwoSum({}, LeetCode::Array::TwoSum::two({6}, 6)));
    //EXPECT_TRUE(verifyTwoSum({}, LeetCode::Array::TwoSum::two({2, 7, 11, 15}, 6)));
    EXPECT_EQ(1, verifyTwoSum({}, LeetCode::Array::TwoSum::two({2, 7, 11, 15}, 6)) ? 1 : 0);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}