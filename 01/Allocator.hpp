#include <stdlib.h>

class Allocator {
   private:
    char* data_;
    size_t offset_;
    size_t max_size_;

   public:
    Allocator() : data_(nullptr), offset_(0), max_size_(0) {}

    void MakeAllocator(size_t max_size);
    char* Alloc(size_t size);
    void Reset();

    ~Allocator();
};