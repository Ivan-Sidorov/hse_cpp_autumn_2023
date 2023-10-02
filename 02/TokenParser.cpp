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
    this->start_callback_();
    bool is_digit = true;
    std::string token = "";
    char symbol;
    for (size_t i = 0; i <= str.size(); ++i) {
        if (i == str.size()) {
            symbol = ' ';
        } else {
            symbol = str[i];
        }
        if (!this->separators.contains(symbol)) {
            token += symbol;
            if (!std::isdigit(symbol)) {
                is_digit = false;
            }
        } else if (!is_digit) {
            this->string_callback_(token);
            token = "";
            is_digit = true;
        } else if (token != "") {
            try {
                uint64_t digit_token = std::stoul(token);
                this->digit_callback_(digit_token);
            } catch (std::out_of_range) {
                this->string_callback_(token);
            }
            token = "";
            is_digit = true;
        }
    }
    this->end_callback_();
}