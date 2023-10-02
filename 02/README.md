# Домашнее задание к лекции №3

## Библиотека-парсер строк

Класс содержит следующие методы:
```c++
class TokenParser {
   public:
    TokenParser()
        : start_callback_([]() {}),
          end_callback_([]() {}),
          digit_callback_([](uint64_t) {}),
          string_callback_([](const std::string &) {}){};

    // Устанавливаем callback-функцию перед стартом парсинга.
    void SetStartCallback(std::function<void()> callback);

    // Устанавливаем callback-функцию после окончания парсинга.
    void SetEndCallback(std::function<void()> callback);

    // Устанавливаем callback-функцию для обработки чисел.
    void SetDigitTokenCallback(std::function<void(uint64_t)> callback);

    // Устанавливаем callback-функцию для обработки строк.
    void SetStringTokenCallback(std::function<void(std::string)> callback);

    void Parse(const std::string &str);
};
```

`Makefile` содержит цель-тест:
```bash
make
make test
make clean
```
