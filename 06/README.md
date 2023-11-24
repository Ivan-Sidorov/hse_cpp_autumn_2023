# Домашнее задание к лекции №7

## Форматироаание строк

Пример использования:
```c++
std::string text = format("{1}+{1} = {0}", 2, "one");
// text = "one+one = 2"
```

`Makefile` содержит цель-тест:
```bash
make
make test
make clean
```