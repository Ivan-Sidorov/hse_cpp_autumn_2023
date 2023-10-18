# Домашнее задание к лекции №5

## Класс, реализующий длинную арифметику

Класс содержит следующие методы:
```c++
class BigInt {
   public:
    BigInt();
    BigInt(const std::string& str);
    BigInt(int32_t num);
    BigInt(const BigInt& copied);
    BigInt(BigInt&& moved);
    ~BigInt();

    BigInt& operator=(const BigInt& copied);
    BigInt& operator=(BigInt&& moved);
    BigInt& operator=(const int32_t& num);

    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    BigInt operator-() const;

    BigInt operator+(const BigInt& other) const;
    BigInt operator+(const int32_t& other) const;

    BigInt operator-(const BigInt& other) const;
    BigInt operator-(const int32_t& other) const;

    BigInt operator*(const BigInt& other) const;
    BigInt operator*(const int32_t& other) const;

    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);
};
```

`Makefile` содержит цель-тест:
```bash
make
make test
make clean
```