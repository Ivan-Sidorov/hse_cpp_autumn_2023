#include "TokenParser.hpp"

void TokenParser::SetStartCallback(std::function<void()> callback) {
    this->start_callback_ = callback;
}

void TokenParser::SetEndCallback(std::function<void()> callback) {
    this->end_callback_ = callback;
}

void TokenParser::SetDigitTokenCallback(
    std::function<void(uint64_t)> callback) {
    this->digit_callback_ = callback;
}

void TokenParser::SetStringTokenCallback(
    std::function<void(std::string)> callback) {
    this->string_callback_ = callback;
}

void TokenParser::Parse(const std::string &str) {
    // Вызываем функцию, исполняемую перед парсингом
    this->start_callback_();
    bool is_digit = true;
    std::string token = "";
    char symbol;
    // Проходимся по всем символам
    for (size_t i = 0; i <= str.size(); ++i) {
        if (i == str.size()) {
            // Нужно, чтобы обработать последний токен в строке
            symbol = ' ';
        } else {
            symbol = str[i];
        }
        // Смотрим не является ли токен разделителем
        if (!this->separators.contains(symbol)) {
            // Добавляем символ в выделяемый токен
            token += symbol;
            // Если встретили хоть один символ не цифру
            // Значит токен уже будет обрабатываться как строка
            if (!std::isdigit(symbol)) {
                is_digit = false;
            }
        } else if (!is_digit) {
            // Встретили символ разделитель и в токене есть не символ-цифра
            // Вызываем обработчик для строки
            this->string_callback_(token);
            token = "";
            is_digit = true;
        } else if (token != "") {
            // Остается вариант, что токен является числом
            // Пробуем преобразовать строку в число
            // И вызвать соответствующий callback
            try {
                uint64_t digit_token = std::stoul(token);
                this->digit_callback_(digit_token);
            } catch (const std::out_of_range &) {
                // Число не помещается в uint64_t
                // Вызываем обработчик строки
                this->string_callback_(token);
            }
            token = "";
            is_digit = true;
        }
    }
    // Вызываем функцию, исполняемую после парсинга
    this->end_callback_();
}