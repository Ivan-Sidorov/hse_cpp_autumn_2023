#include "BigInt.hpp"

BigInt::BigInt() : digits_(nullptr), sign_(1), size_(0) {}

BigInt::BigInt(const std::string& str) {
    if (!str.empty()) {
        int start = 0;
        if (str[0] == '-') {
            sign_ = -1;
            start = 1;
        } else if (str[0] == '+') {
            sign_ = 1;
            start = 1;
        } else {
            sign_ = 1;
        }

        size_ = str.size() - start;
        digits_ = new uint8_t[size_];
        // переводим символы в числа через юникод
        // и записываем в обратном порядке в массив
        for (size_t i = start; i < str.size(); ++i) {
            uint8_t digit = str[str.size() - i - 1 + start] - '0';
            if (digit > 9) {
                throw std::invalid_argument(
                    "This string can't be converted to a number.");
            }
            digits_[i - start] = digit;
        }
    } else {
        digits_ = nullptr;
        size_ = 0;
        sign_ = 1;
    }
}

BigInt::BigInt(int32_t num) {
    *this = num;
    if (num == 0) {
        digits_ = new uint8_t[1];
        digits_[0] = 0;
        size_ = 1;
        sign_ = 1;
    } else {
        if (num < 0) {
            sign_ = -1;
            num = -num;
        } else {
            sign_ = 1;
        }
        size_ = 0;
        int32_t tmp_num = num;
        // через div 10 узнаем размера числа
        while (tmp_num > 0) {
            tmp_num /= 10;
            size_++;
        }
        digits_ = new uint8_t[size_];
        tmp_num = num;
        // через mod берем последнюю цифру
        // через div отбрасываем
        // число пишется в обратном порядке
        for (size_t i = 0; i < size_; ++i) {
            digits_[i] = tmp_num % 10;
            tmp_num /= 10;
        }
    }
}

BigInt::BigInt(const BigInt& copied) { *this = copied; }

BigInt::BigInt(BigInt&& moved) { *this = std::move(moved); }

BigInt::~BigInt() { delete[] digits_; }

BigInt& BigInt::operator=(const BigInt& copied) {
    size_ = copied.size_;
    sign_ = copied.sign_;
    digits_ = new uint8_t[size_];
    for (size_t i = 0; i < size_; ++i) {
        digits_[i] = copied.digits_[i];
    }

    return *this;
}

BigInt& BigInt::operator=(const int32_t& num) {
    if (num == 0) {
        digits_ = new uint8_t[1];
        digits_[0] = 0;
        size_ = 1;
        sign_ = 1;
    } else {
        if (num < 0) {
            sign_ = -1;
        } else {
            sign_ = 1;
        }
        size_ = 0;
        int32_t tmp_num = num;
        if (num < 0) {
            tmp_num = -tmp_num;
        }
        while (tmp_num > 0) {
            tmp_num /= 10;
            size_++;
        }
        digits_ = new uint8_t[size_];
        tmp_num = num;
        if (num < 0) {
            tmp_num = -tmp_num;
        }
        for (size_t i = 0; i < size_; ++i) {
            digits_[i] = tmp_num % 10;
            tmp_num /= 10;
        }
    }

    return *this;
}

BigInt& BigInt::operator=(BigInt&& moved) {
    size_ = moved.size_;
    sign_ = moved.sign_;
    digits_ = new uint8_t[size_];
    for (size_t i = 0; i < size_; ++i) {
        digits_[i] = moved.digits_[i];
    }

    moved.size_ = 0;
    moved.sign_ = 1;
    delete[] moved.digits_;
    moved.digits_ = nullptr;

    return *this;
}

bool BigInt::operator==(const BigInt& other) const {
    if (size_ != other.size_ || sign_ != other.sign_) {
        return false;
    }
    // поэлементное сравнение
    for (size_t i = 0; i < size_; ++i) {
        if (digits_[i] != other.digits_[i]) {
            return false;
        }
    }
    return true;
}

bool BigInt::operator!=(const BigInt& other) const { return !(*this == other); }

bool BigInt::operator<(const BigInt& other) const {
    if (sign_ < other.sign_) {
        return true;
    } else if (sign_ > other.sign_) {
        return false;
    } else if (size_ < other.size_) {
        return sign_ > 0;
    } else if (size_ > other.size_) {
        return sign_ < 0;
    } else {
        for (size_t i = size_; i-- > 0;) {
            if (digits_[i] < other.digits_[i]) {
                return sign_ > 0;
            } else if (digits_[i] > other.digits_[i]) {
                return sign_ < 0;
            }
        }
        return false;
    }
}

bool BigInt::operator<=(const BigInt& other) const {
    // используем реализованные операторы < и ==
    return (*this < other) || (*this == other);
}

bool BigInt::operator>(const BigInt& other) const {
    // используем оператор <=
    return !(*this <= other);
}

bool BigInt::operator>=(const BigInt& other) const {
    // используем оператор <
    return !(*this < other);
}

BigInt BigInt::operator-() const {
    // знак минус перед числом
    BigInt result(*this);
    result.sign_ = -sign_;
    return result;
}

void ResizeResult(uint8_t*& digits, size_t& size) {
    // очищаем лишнюю аллоцированную память
    size_t zero_counter = 0;
    size_t tmp_size = size;
    // считаем количество оставшихся нулей
    while (tmp_size > 1 && digits[tmp_size - 1] == 0) {
        zero_counter++;
        tmp_size--;
    }
    // аллоцируем память нужного размера, переносим туда элементы
    // старую освобождаем
    if (zero_counter > 0) {
        size -= zero_counter;
        uint8_t* digits_new = new uint8_t[size];
        for (size_t i = 0; i < size; ++i) {
            digits_new[i] = digits[i];
        }
        delete[] digits;
        digits = digits_new;
    }
}

BigInt BigInt::operator+(const BigInt& other) const {
    if (sign_ == other.sign_) {
        BigInt result;
        // аллоцируем память на 1 больше, чем макс. размер
        // на случай перехода в новый разряд
        result.size_ = std::max(size_, other.size_) + 1;
        result.digits_ = new uint8_t[result.size_];
        for (size_t i = 0; i < result.size_; ++i) {
            result.digits_[i] = 0;
        }

        int carry = 0;
        for (size_t i = 0; i < result.size_; i++) {
            int sum = carry;
            // проверка, что цифры в числе еще не кончились
            if (i < size_) {
                sum += digits_[i];
            }
            if (i < other.size_) {
                sum += other.digits_[i];
            }
            // берем mod 10, остальное в остаток carry
            result.digits_[i] = sum % 10;
            carry = sum / 10;
        }
        ResizeResult(result.digits_, result.size_);
        result.sign_ = sign_;
        return result;
    } else if (*this > other) {
        return *this - (-other);
    } else {
        return other - (-*this);
    }
}

BigInt BigInt::operator+(const int32_t& other) const {
    return *this + BigInt(other);
}

BigInt BigInt::operator-(const BigInt& other) const {
    if ((sign_ > 0) && (other.sign_ > 0)) {
        if (*this >= other) {
            BigInt result;
            result.size_ = size_;
            result.digits_ = new uint8_t[result.size_];
            for (size_t i = 0; i < result.size_; ++i) {
                result.digits_[i] = 0;
            }

            int carry = 0;
            for (size_t i = 0; i < size_; ++i) {
                int diff = carry + digits_[i];
                // проверка, что цифры в числе еще не кончились
                if (i < other.size_) {
                    diff -= other.digits_[i];
                }
                // занимаем из старшего разряда
                if (diff < 0) {
                    diff += 10;
                    carry = -1;
                } else {
                    carry = 0;
                }
                result.digits_[i] = diff;
            }
            ResizeResult(result.digits_, result.size_);
            result.sign_ = sign_;
            return result;
        } else {
            return -(other - *this);
        }
    } else if ((sign_ < 0) && (other.sign_ < 0)) {
        return -(-*this - (-other));
    } else if (other.sign_ < 0) {
        return *this + (-other);
    } else {
        return -(-*this + other);
    }
}

BigInt BigInt::operator-(const int32_t& other) const {
    return *this - BigInt(other);
}

BigInt BigInt::operator*(const BigInt& other) const {
    BigInt result;
    result.size_ = size_ + other.size_;
    result.digits_ = new uint8_t[result.size_];
    for (size_t i = 0; i < result.size_; ++i) {
        result.digits_[i] = 0;
    }

    for (size_t i = 0; i < size_; ++i) {
        int carry = 0;
        for (size_t j = 0; j < other.size_ || carry > 0; ++j) {
            // как и при умножении столбиком, сдвигаемся на разряд
            // остаток переносим в старший разряд
            // также обрабатываем случай, когда еще есть, что перенести в
            // старший разряд а само число уже кончилось (умножаем просто на 0)
            int prod = carry + result.digits_[i + j] +
                       digits_[i] * (j < other.size_ ? other.digits_[j] : 0);
            result.digits_[i + j] = prod % 10;
            carry = prod / 10;
        }
    }
    ResizeResult(result.digits_, result.size_);
    // обрабатываем умножение на 0
    if (!(result.size_ == 1) || !(result.digits_[0] == 0)) {
        result.sign_ = sign_ * other.sign_;
    } else {
        result.sign_ = 1;
    }
    return result;
}

BigInt BigInt::operator*(const int32_t& other) const {
    return *this * BigInt(other);
}

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    if ((num.sign_ == -1) && !((num.size_ == 1) && (num.digits_[0] == 0))) {
        os << '-';
    }
    // выводим, читая массив в обратном порядке
    // size_t – неотрицателен, так что тут нестандартное условие завершения
    for (size_t i = num.size_; i-- > 0;) {
        os << unsigned(num.digits_[i]);
    }
    return os;
}