#pragma once
#include <iostream>
#include <string>

#include "Error.hpp"

class Deserializer {
    static constexpr char Separator = ' ';

   public:
    explicit Deserializer(std::istream& in) : in_(in) {}

    template <class T>
    Error load(T& object) {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args) {
        return process(args...);
    }

   private:
    std::istream& in_;

    template <class T, class... Args>
    Error process(T& value, Args&... args) {
        Error err = process(value);
        // проверка на возвращаемое значение
        if (err == Error::CorruptedArchive) {
            return err;
        }

        // рекурсивно запускаем для остальных аргументов
        return process(args...);
    }

    Error process(bool& arg) {
        std::string text;
        in_ >> text;

        // если пришла строка true/false, возвращаем соответствующие значения
        // иначе возвращаем ошибку Error::CorruptedArchive
        if (text == "true") {
            arg = true;
        } else if (text == "false") {
            arg = false;
        } else {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

    Error process(uint64_t& arg) {
        std::string text;
        in_ >> text;

        // пытаемся перевести полученную строку в число
        // если не вышло возвращаем Error::CorruptedArchive
        try {
            arg = std::stoull(text);
        } catch (...) {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }
};