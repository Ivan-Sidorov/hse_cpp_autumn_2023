#include "Allocator.hpp"

void Allocator::MakeAllocator(size_t max_size) {
    // если идет попытка заново создать аллокатор,
    // необходимо почистить выделенную до этого память
    delete[] this->data_;

    this->data_ = new char[max_size];
    this->offset_ = 0;
    this->max_size_ = max_size;
}

char* Allocator::Alloc(size_t size) {
    // если текущий сдвиг + размер выделяемы памяти превышает допустимый
    // или попытка выделить память размером 0, возвращаем nullptr
    if ((this->offset_ + size > this->max_size_) || (size == 0)) {
        return nullptr;
    }

    char* result = nullptr;
    // выделяем память с учетом сдвига (уже выделенной)
    result = this->data_ + this->offset_;
    // обновляем размер сдвига
    this->offset_ += size;

    return result;
}

void Allocator::Reset() {
    // при сбросе выделенной памяти просто обнуляем сдвиг
    // память оставляем выделенной для дальнейшего использования
    this->offset_ = 0;
}

Allocator::~Allocator() { delete[] this->data_; }