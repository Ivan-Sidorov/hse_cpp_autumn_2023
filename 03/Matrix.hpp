#pragma once
#include <stdlib.h>

#include <iostream>

class Matrix {
   private:
    // прокси-класс для доступа к элементу матрицы через [][]
    // здесь содержатся отдельные строки матрицы длины cols_ (pcols_)
    class ProxyRow {
       private:
        int32_t* pdata_;
        size_t pcols_;

       public:
        ProxyRow() = default;

        void setRow(size_t cols);

        int32_t& operator[](size_t j);

        ~ProxyRow();
    };

    size_t rows_;
    size_t cols_;
    ProxyRow* data_;

   public:
    Matrix() = default;
    Matrix(size_t rows, size_t cols);

    size_t getRows();
    size_t getColumns();

    ProxyRow& operator[](size_t i);
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
    Matrix& operator*=(int num);
    Matrix operator+(Matrix& other);
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    ~Matrix();
};