#include <functional>
#include <iostream>
#include <set>
#include <string>

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

    void SetStringTokenCallback(std::function<void(std::string)> callback);

    void Parse(const std::string &str);

   private:
    std::function<void()> start_callback_;
    std::function<void()> end_callback_;
    std::function<void(uint64_t)> digit_callback_;
    std::function<void(const std::string &)> string_callback_;
    std::set<char> separators = {' ', '\n', '\t'};
};