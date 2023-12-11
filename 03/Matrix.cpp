#include "Matrix.hpp"

void Matrix::ProxyRow::setRow(size_t cols) {
    pcols_ = cols;
    this->pdata_ = new int32_t[cols];
    for (size_t i = 0; i < cols; ++i) {
        this->pdata_[i] = 0;
    }
}

Matrix::ProxyRow& Matrix::operator[](size_t i) {
    // запрашиваемый индекс строки больше кол-ва строк в матрице
    if (i >= this->rows_) {
        throw std::out_of_range("Index out of range matrix");
    }
    return this->data_[i];
}

Matrix::ProxyRow::~ProxyRow() { delete[] pdata_; }

Matrix::Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols) {
    // храним столько экзепляров класса ProxyRow, сколько у нас строк
    data_ = new ProxyRow[rows];
    for (size_t i = 0; i < rows; ++i) {
        data_[i].setRow(cols);
    }
}

size_t Matrix::getRows() { return this->rows_; }
size_t Matrix::getColumns() { return this->cols_; }

int32_t& Matrix::ProxyRow::operator[](size_t j) {
    // запрашиваемый индекс столбца больше кол-ва столбцов в матрице
    if (j >= this->pcols_) {
        throw std::out_of_range("Index out of range");
    }
    return this->pdata_[j];
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (size_t i = 0; i < matrix.rows_; ++i) {
        for (size_t j = 0; j < matrix.cols_; ++j) {
            os << matrix.data_[i][j] << ' ';
        }
        os << '\n';
    }

    return os;
}

Matrix& Matrix::operator*=(int num) {
    for (size_t i = 0; i < this->rows_; ++i)
        for (size_t j = 0; j < this->cols_; ++j) {
            // умножаем каждый элемент матрицы на число
            this->data_[i][j] *= num;
        }

    return *this;
}

Matrix Matrix::operator+(Matrix& other) {
    // размеры матриц не совпадают
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::out_of_range("Matrices have different sizes");
    }
    Matrix sum(this->rows_, this->cols_);
    for (size_t i = 0; i < this->rows_; ++i)
        for (size_t j = 0; j < this->cols_; ++j) {
            // суммируем элементы с одинаковыми индексами
            sum.data_[i][j] = this->data_[i][j] + other.data_[i][j];
        }

    return sum;
}

bool Matrix::operator==(const Matrix& other) const {
    // сразу проверяем одинаковые ли размеры у матрицы
    if (this->rows_ != other.rows_ || this->cols_ != other.cols_) {
        throw std::out_of_range("Matrices have different sizes");
    }
    for (size_t i = 0; i < this->rows_; ++i)
        for (size_t j = 0; j < this->cols_; ++j) {
            // сравниваем попарно все члены матриц
            if (this->data_[i][j] != other.data_[i][j]) {
                return false;
            }
        }

    return true;
}

bool Matrix::operator!=(const Matrix& other) const {
    // просто используем оператор == с отрицанием
    return !(*this == other);
}

Matrix::~Matrix() { delete[] data_; }