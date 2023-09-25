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
    EXPECT_NE(nullptr, allocator.Alloc(3));
    EXPECT_NE(nullptr, allocator.Alloc(2));
}

TEST(AllocatorTest, AllocOverflowTest) {
    Allocator allocator;
    allocator.MakeAllocator(5);
    EXPECT_EQ(nullptr, allocator.Alloc(6));

    EXPECT_NE(nullptr, allocator.Alloc(3));
    EXPECT_EQ(nullptr, allocator.Alloc(3));
}

TEST(AllocatorTest, ResetTest) {
    Allocator allocator;
    allocator.MakeAllocator(5);
    EXPECT_NE(nullptr, allocator.Alloc(3));
    allocator.Reset();
    EXPECT_NE(nullptr, allocator.Alloc(3));
}

TEST(AllocatorTest, ZeroAllocTest) {
    Allocator allocator;
    allocator.MakeAllocator(5);
    EXPECT_EQ(nullptr, allocator.Alloc(0));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}