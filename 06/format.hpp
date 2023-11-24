#include <sstream>
#include <string>

#include "exceptions.hpp"

template <class T>
int GetArg(std::vector<std::string>& str_args, T& arg) {
    std::stringstream ss;
    ss << arg;
    str_args.push_back(ss.str());

    return 0;
}

template <class T, class... ArgsT>
int GetArg(std::vector<std::string>& str_args, T& arg, ArgsT&&... args) {
    int res = GetArg(str_args, arg);
    GetArg(str_args, std::forward<ArgsT>(args)...);

    return 0;
}

template <class... ArgsT>
std::string format(std::string str, ArgsT... args) {
    size_t open_idx, close_idx;

    // Получаем массив аргументов
    std::vector<std::string> str_args;
    GetArg(str_args, args...);
    size_t n_args = str_args.size();

    if (str[0] == '}') {
        throw BraceException("Invalid curly braces usage!");
    }

    size_t i = 0;
    while (i < str.length()) {
        // Идем по индексам, пока не встретим открытую скобку
        while (str[i] != '{') {
            ++i;
            // Дошли до конца и не встретили "{"
            if (i == str.length()) {
                return str;
            }

            if (str[i] == '}') {
                throw BraceException("Invalid curly braces usage!");
            }
        }
        // Запоминаем индекс начала форматирования
        open_idx = i;

        // Идем по индексам, пока не встретим закрытую скобку
        // Поиск выполняется при наличии открытой скобки
        while (str[i] != '}') {
            ++i;
            // Дошли до конца и не встретили "}"
            if (i == str.length()) {
                // Здесь нужно выбросить исключение
                // Скобку открыли, но не закрыли
                throw BraceException("Invalid curly braces usage!");
            }

            if (str[i] == '{') {
                throw BraceException("Invalid curly braces usage!");
            }
        }
        close_idx = i;

        // Получаем индекс аргумента в скобках
        std::string arg_idx =
            str.substr(open_idx + 1, close_idx - open_idx - 1);

        if (arg_idx.empty()) {
            throw ArgException(
                "Invalid argument index. Only positive integers are allowed!");
        }

        // Если индекс аргумента не число, бросаем исключение
        bool is_digit = true;
        for (char const& ch : arg_idx) {
            if (std::isdigit(ch) == 0) {
                is_digit = false;
                break;
            };
        }
        if (!is_digit) {
            throw ArgException(
                "Invalid argument index. Only positive integers are allowed!");
        }

        uint32_t int_arg_idx = std::stoul(arg_idx);

        // Если индекс аргумента больше, чем их количество
        if (int_arg_idx >= n_args) {
            throw ArgException(
                "Invalid argument index. The specified index is greater than "
                "the number of arguments!");
        }

        // Вставляем аргумент вместо индекса
        str.erase(open_idx, close_idx - open_idx + 1);
        str.insert(open_idx, str_args[int_arg_idx]);
    }
    return str;
}