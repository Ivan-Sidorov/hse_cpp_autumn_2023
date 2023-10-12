#include "Matrix.hpp"
#include "gtest/gtest.h"

TEST(MatrixTest, GetterTest) {
    Matrix matrix(3, 4);
    ASSERT_EQ(matrix.getRows(), 3);
    ASSERT_EQ(matrix.getColumns(), 4);
}

TEST(MatrixTest, ElementAccessTest) {
    Matrix matrix(2, 2);
    matrix[0][1] = 42;

    ASSERT_EQ(matrix[0][1], 42);
}

TEST(MatrixTest, ComparisonTest) {
    size_t rows = 3;
    size_t cols = 3;
    Matrix matrix1(rows, cols);
    Matrix matrix2(rows, cols);
    Matrix matrix3(rows, cols);

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j) {
            matrix1[i][j] = i + j;
            matrix2[i][j] = i + j;
        }

    ASSERT_EQ(matrix1 == matrix2, true);
    ASSERT_EQ(matrix1 != matrix3, true);
}

TEST(MatrixTest, OutputTest) {
    size_t rows = 3;
    size_t cols = 3;
    Matrix matrix(rows, cols);

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j) {
            matrix[i][j] = i + j;
        }

    ::testing::internal::CaptureStdout();
    std::cout << matrix;
    std::string output = ::testing::internal::GetCapturedStdout();
    std::string expected = "0 1 2 \n1 2 3 \n2 3 4 \n";
    ASSERT_EQ(output, expected);
}

TEST(MatrixTest, MultiplyTest) {
    Matrix matrix(2, 2);
    matrix[1][1] = 21;
    matrix *= 2;
    ASSERT_EQ(matrix[0][0], 0);
    ASSERT_EQ(matrix[0][1], 0);
    ASSERT_EQ(matrix[1][0], 0);
    ASSERT_EQ(matrix[1][1], 42);
}

TEST(MatrixTest, SumTest) {
    size_t rows = 3;
    size_t cols = 3;
    Matrix matrix1(rows, cols);
    Matrix matrix2(rows, cols);

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j) {
            matrix1[i][j] = i + j;
            matrix2[i][j] = i + j;
        }

    Matrix matrix_sum = matrix1 + matrix2;
    ::testing::internal::CaptureStdout();
    std::cout << matrix_sum;
    std::string output = ::testing::internal::GetCapturedStdout();
    std::string expected = "0 2 4 \n2 4 6 \n4 6 8 \n";
    ASSERT_EQ(output, expected);
}

TEST(MatrixTest, OutOfRangeTest) {
    Matrix matrix(3, 4);
    EXPECT_THROW({ matrix[10][2] = 10; }, std::out_of_range);
    EXPECT_THROW({ matrix[2][10] = 10; }, std::out_of_range);
}

TEST(MatrixTest, OutOfRangeSumTest) {
    Matrix matrix1(5, 5);
    Matrix matrix2(3, 3);
    EXPECT_THROW({ Matrix matrix_sum = matrix1 + matrix2; }, std::out_of_range);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}