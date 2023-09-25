#include "Allocator.hpp"

void Allocator::MakeAllocator(size_t max_size) {
    delete[] data_;

    char* data = new char[max_size];
    offset_ = 0;
    this->max_size_ = max_size;
}

char* Allocator::Alloc(size_t size) {
    if (offset_ + size > this->max_size_) {
        return nullptr;
    }
    offset_ += size;
    return data_ + size;
}

void Allocator::Reset() { offset_ = 0; }

Allocator::~Allocator() { delete[] data_; }