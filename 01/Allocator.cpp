#include "Allocator.hpp"

void Allocator::MakeAllocator(size_t max_size) {
    delete[] this->data_;

    this->data_ = new char[max_size];
    this->offset_ = 0;
    this->max_size_ = max_size;
}

char* Allocator::Alloc(size_t size) {
    if ((this->offset_ + size > this->max_size_) || (size == 0)) {
        return nullptr;
    }
    this->offset_ += size;
    return this->data_ + size;
}

void Allocator::Reset() { this->offset_ = 0; }

Allocator::~Allocator() { delete[] this->data_; }