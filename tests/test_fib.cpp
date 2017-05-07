/* Tests for fib. */

#include "gtest/gtest.h"
#include "fib.h"

TEST(FibTest, ZerothNumberIsZero) {
    EXPECT_EQ(fib(0), 0);
}

TEST(FibTest, FirstNumberIsOne) {
    EXPECT_EQ(fib(1), 1);
}

TEST(FibTest, SecondIsOne) {
    EXPECT_EQ(fib(2), 1);
}

TEST(FibTest, ThirdIsTwo) {
    EXPECT_EQ(fib(3), 2);
}

TEST(FibTest, FourthIsThree) {
    EXPECT_EQ(fib(4), 3);
}

TEST(FibTest, FifthIsFive) {
    EXPECT_EQ(fib(5), 5);
}
