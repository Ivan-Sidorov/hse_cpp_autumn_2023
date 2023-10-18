#include "BigInt.hpp"
#include "gtest/gtest.h"

TEST(BigIntTest, InitConstructorsTest) {
    BigInt num1(1234);
    ::testing::internal::CaptureStdout();
    std::cout << num1;
    std::string output1 = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output1, "1234");

    BigInt num2(-1234);
    ::testing::internal::CaptureStdout();
    std::cout << num2;
    std::string output2 = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output2, "-1234");

    BigInt num3("1234");
    ::testing::internal::CaptureStdout();
    std::cout << num3;
    std::string output3 = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output3, "1234");

    BigInt num4("-1234");
    ::testing::internal::CaptureStdout();
    std::cout << num4;
    std::string output4 = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output4, "-1234");

    EXPECT_THROW({ BigInt num5("-12s34"); }, std::invalid_argument);
}

TEST(BigIntTest, CopyMoveConstructorsTest) {
    BigInt num1 = 42;
    ::testing::internal::CaptureStdout();
    std::cout << num1;
    std::string output1 = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output1, "42");

    BigInt num2 = std::move(num1);
    ::testing::internal::CaptureStdout();
    std::cout << num2;
    std::string output2 = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output2, "42");
}

TEST(BigIntTest, CopyMoveOperatorsTest) {
    BigInt num1("1234");
    BigInt num2(42);
    num2 = num1;
    ::testing::internal::CaptureStdout();
    std::cout << num2;
    std::string output1 = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output1, "1234");

    BigInt num3("1234");
    BigInt num4(42);
    num4 = std::move(num3);
    ::testing::internal::CaptureStdout();
    std::cout << num4;
    std::string output2 = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output2, "1234");

    BigInt num5("1234");
    num5 = 4321;
    ::testing::internal::CaptureStdout();
    std::cout << num5;
    std::string output3 = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output3, "4321");
}

TEST(BigIntTest, ComparisonOperatorsTest) {
    BigInt num1 = 42;
    BigInt num2 = -1234;
    ASSERT_EQ(num1 > num2, true);
    ASSERT_EQ(num1 >= num2, true);
    ASSERT_EQ(num1 < num2, false);
    ASSERT_EQ(num1 <= num2, false);
    ASSERT_EQ(num1 == num2, false);
    ASSERT_EQ(num1 != num2, true);
}

TEST(BigIntTest, SumOperatorTest) {
    BigInt num1("123456789012345678901234567890");
    BigInt num2("-123456789012345678901234567891");
    ASSERT_EQ(num1 + num2, -1);
    ASSERT_EQ(num2 + num1, -1);

    BigInt num3("123456789012345678901234567890");
    BigInt num4("9876543210987654321");
    BigInt expected1("123456789022222222112222222211");
    ASSERT_EQ((num3 + num4) == expected1, true);

    BigInt num5("-123456789012345678901234567890");
    BigInt num6("-9876543210987654321");
    BigInt expected2("-123456789022222222112222222211");
    ASSERT_EQ((num5 + num6) == expected2, true);
}

TEST(BigIntTest, SubOperatorTest) {
    BigInt num1("123456789012345678901234567890");
    BigInt num2("-123456789012345678901234567891");
    BigInt expected1("246913578024691357802469135781");
    ASSERT_EQ(num1 - num2, expected1);
    ASSERT_EQ(num2 - num1, -expected1);

    BigInt num3("123456789012345678901234567890");
    BigInt num4("9876543210987654321");
    BigInt expected2("123456789002469135690246913569");
    ASSERT_EQ((num3 - num4) == expected2, true);
    ASSERT_EQ((num4 - num3) == -expected2, true);

    BigInt num5("-123456789012345678901234567890");
    BigInt num6("-9876543210987654321");
    BigInt expected3("-123456789002469135690246913569");
    ASSERT_EQ((num5 - num6) == expected3, true);
    ASSERT_EQ((num6 - num5) == -expected3, true);
}

TEST(BigIntTest, MulOperatorTest) {
    BigInt num1("123456789");
    BigInt num2("-555777");
    BigInt expected1("-68614443820053");
    ASSERT_EQ(num1 * num2, expected1);

    BigInt num3("123456789");
    BigInt num4("987654321");
    BigInt expected2("121932631112635269");
    ASSERT_EQ((num3 * num4) == expected2, true);

    BigInt num5("-123456789");
    BigInt num6("-987654321");
    BigInt expected3("121932631112635269");
    ASSERT_EQ((num5 * num6) == expected3, true);

    BigInt num7("-123456789");
    BigInt num8("0");
    ASSERT_EQ((num7 * num8) == 0, true);
}

TEST(BigIntTest, IntOperatorsTest) {
    BigInt num1("1234");
    int32_t num2 = 5678;

    ASSERT_EQ(num1 + num2, 6912);
    ASSERT_EQ(num1 - num2, -4444);
    ASSERT_EQ(num1 * num2, 7006652);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}