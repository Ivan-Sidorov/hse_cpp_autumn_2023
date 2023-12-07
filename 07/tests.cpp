#include <gtest/gtest.h>

#include <string>

#include "AVL.hpp"

TEST(TreeTest, EmptyTreeTest) {
    AVL<int, std::string> tree;
    ASSERT_EQ(tree.empty(), 1);

    tree.insert(42, "42");
    ASSERT_EQ(tree.empty(), 0);
}

TEST(TreeTest, InsertionTest) {
    AVL<int, std::string> tree;
    tree.insert(42, "42");
    tree.insert(24, "24");
    tree.insert(17, "17");
    tree.insert(5, "5");
    tree.insert(84, "84");
    ASSERT_EQ(tree.size(), 5);
    ASSERT_EQ(tree.contains(17), 1);
    ASSERT_EQ(tree.contains(15), 0);

    EXPECT_THROW({ tree.insert(42, "42"); }, std::invalid_argument);
}

TEST(TreeTest, DeletionTest) {
    AVL<int, std::string> tree;
    tree.insert(42, "42");
    tree.insert(24, "24");
    tree.insert(17, "17");
    tree.insert(5, "5");
    tree.insert(84, "84");
    ASSERT_EQ(tree.size(), 5);

    tree.erase(5);
    ASSERT_EQ(tree.size(), 4);
    ASSERT_EQ(tree.contains(5), 0);

    EXPECT_THROW({ tree.erase(555); }, std::invalid_argument);

    tree.clear();
    ASSERT_EQ(tree.empty(), 1);
}

TEST(TreeTest, IteratorTest) {
    AVL<int, std::string> tree;
    tree.insert(42, "42");
    tree.insert(24, "24");
    tree.insert(17, "17");
    tree.insert(5, "5");
    tree.insert(84, "84");

    std::string begin = *tree.begin();
    std::string end = *tree.end();
    ASSERT_EQ(begin, "5");
    ASSERT_EQ(end, "84");

    std::string rbegin = *tree.rbegin();
    std::string rend = *tree.rend();
    ASSERT_EQ(rbegin, "84");
    ASSERT_EQ(rend, "5");
}

TEST(TreeTest, BracesOperatorTest) {
    AVL<int, std::string> tree;
    tree.insert(42, "42");
    tree.insert(24, "24");
    tree.insert(17, "17");
    tree.insert(5, "5");
    tree.insert(84, "84");

    tree[3] = "42 edited";
    auto node = tree.find(tree.root_, 42);
    ASSERT_EQ(node->value_, "42 edited");

    EXPECT_THROW({ tree[100500] = "error"; }, std::invalid_argument);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}