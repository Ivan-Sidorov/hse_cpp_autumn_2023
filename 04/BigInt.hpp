#pragma once
#include <iostream>
#include <string>

class BigInt {
   private:
    uint8_t* digits_;
    int sign_;
    size_t size_;

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