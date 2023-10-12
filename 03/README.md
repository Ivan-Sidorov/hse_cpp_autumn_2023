# Домашнее задание к лекции №4

## Класс-матрица с поддержкой операций

Класс содержит следующие методы:
```c++
class Matrix {
   public:
    size_t getRows();
    size_t getColumns();

    ProxyRow& operator[](size_t i);
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
    Matrix& operator*=(int num);
    Matrix operator+(Matrix& other);
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;
};
```

`Makefile` содержит цель-тест:
```bash
make
make test
make clean
```