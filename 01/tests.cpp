#include "Allocator.hpp"
#include "gtest/gtest.h"

TEST(AllocatorTest, MakeAllocatorTest) {
    Allocator allocator;
    allocator.MakeAllocator(5);
    EXPECT_NE(nullptr, allocator.Alloc(3));
}

TEST(AllocatorTest, AdditionalAllocTest) {
    Allocator allocator;
    allocator.MakeAllocator(5);
    char* first = allocator.Alloc(2);
    char* second = allocator.Alloc(2);

    // ожидается, что оба указателя не null, так как переполнения не было
    EXPECT_NE(nullptr, first);
    EXPECT_NE(nullptr, second);

    // проверка на то, что выделяется не один и тот же участок памяти
    EXPECT_NE(first, second);
}

TEST(AllocatorTest, AllocOverflowTest) {
    Allocator allocator;
    allocator.MakeAllocator(5);
    EXPECT_EQ(nullptr, allocator.Alloc(6));

    // при первом аллоцировании все должно быть ок
    // при втором должно вознкнуть переполнение и вернуться nullptr
    EXPECT_NE(nullptr, allocator.Alloc(3));
    EXPECT_EQ(nullptr, allocator.Alloc(3));
}

TEST(AllocatorTest, ResetTest) {
    Allocator allocator;
    allocator.MakeAllocator(5);
    EXPECT_NE(nullptr, allocator.Alloc(3));
    // если выделенная память очистилась, то получится выделить
    // еще раз память размером 3, и не вернутся nullptr
    allocator.Reset();
    EXPECT_NE(nullptr, allocator.Alloc(3));
}

TEST(AllocatorTest, ZeroAllocTest) {
    Allocator allocator;
    allocator.MakeAllocator(5);
    // при попытке выделить память размером 0
    // ожидается nullptr
    EXPECT_EQ(nullptr, allocator.Alloc(0));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}