# Домашнее задание к лекции №2

## Класс аллокатора со стратегией линейного выделения памяти

Класс содержит следующие методы:
```c++
class Allocator {
public:
    void MakeAllocator(size_t MaxSize);
    char* Alloc(size_t size);
    void Reset();

    ~Allocator();
};
```

`Makefile` содержит цель-тест, для запуска выполнить команду:
```bash
make
```