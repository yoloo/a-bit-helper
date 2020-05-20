//
// Created by Konstantin Gredeskoul on 5/16/17.
//
#include "division.h"
#include "gtest/gtest.h"

class DividerTest : public ::testing::Test
{
protected:
    std::vector<long long> numerators   = {5, 9, 17, 933345453464353416L};
    std::vector<long long> denominators = {2, 3, 19, 978737423423423499L};
    std::vector<long long> divisions    = {2, 3, 0, 0};
    std::vector<long long> remainders   = {1, 0, 17, 933345453464353416};

    virtual void SetUp() {};
    virtual void TearDown() {};

    virtual void verify(int index)
    {
        Division::Fraction       fraction = Division::Fraction{numerators.at(index), denominators.at(index)};
        Division::DivisionResult expected = Division::DivisionResult{divisions.at(index), remainders.at(index)};
        Division::DivisionResult result   = Division::Division(fraction).divide();

        EXPECT_EQ(result.division, expected.division);
        EXPECT_EQ(result.remainder, expected.remainder);
    }
};

TEST_F(DividerTest, 5_DivideBy_2)
{
  verify(0);
}

TEST_F(DividerTest, 9_DivideBy_3)
{
  verify(1);
}

TEST_F(DividerTest, 17_DivideBy_19)
{
  verify(2);
}

TEST_F(DividerTest, Long_DivideBy_Long)
{
  verify(3);
}

TEST_F(DividerTest, DivisionByZero)
{
    Division::Division d = Division::Division(Division::Fraction{1, 0});
    try
    {
        d.divide();
        FAIL() << "Expected divide() method to throw DivisionByZeroException";
    }
    catch (Division::DivisionByZero const &err)
    {
        EXPECT_EQ(err.what(), DIVISION_BY_ZERO_MESSAGE);
    }
    catch (...)
    {
        FAIL() << "Expected DivisionByZeroException!";
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

