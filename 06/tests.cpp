#include "format.hpp"
#include "gtest/gtest.h"

TEST(FormatterTest, SimpleTest) {
    // обычный слцчай
    ASSERT_EQ(format("{1} + {1} = {0}", 2, "one"), "one + one = 2");
    // доп проверка разных типов и количества аргументов
    ASSERT_EQ(format("{0} + {1} is {2}", 0, "one", true), "0 + one is 1");
    // отсутствие подстановки аргумента
    ASSERT_EQ(format("no braces", 0, "one", true), "no braces");
}

TEST(FormatterTest, InvalidSingleBracesTest) {
    // лишняя скобка в начале
    EXPECT_THROW({ format("{{1} + {1} = {0}", 2, "one"); }, BraceException);
    EXPECT_THROW({ format("}{1} + {1} = {0}", 2, "one"); }, BraceException);
    // лишняя скобка в конце
    EXPECT_THROW({ format("{0} + {0} = {1}{", 2, "one"); }, BraceException);
    EXPECT_THROW({ format("{0} + {0} = {1}}", 2, "one"); }, BraceException);
    // лишняя скобка в середине
    EXPECT_THROW({ format("{0} + {0} = { {1}}", 2, "one"); }, BraceException);
    // незакрытая/неоткрытая скобка
    EXPECT_THROW({ format("{dfvdsc", 2, "one"); }, BraceException);
    EXPECT_THROW({ format("dfvd}sc", 2, "one"); }, BraceException);
}

TEST(FormatterTest, InvalidArgumentBracesTest) {
    // индекс больше числа аргументов
    EXPECT_THROW({ format("{12} + {1} = {0}", 2, "one"); }, ArgException);
    // невалидный индекс
    EXPECT_THROW({ format("{1} + {-1} = {0}", 2, "one"); }, ArgException);
    EXPECT_THROW({ format("{0} + {0} = {qwerty}", 2, "one"); }, ArgException);
    // пустой индекс
    EXPECT_THROW({ format("{} + {0} = {1}", 2, "one"); }, ArgException);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}