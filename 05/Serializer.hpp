#pragma once
#include <iostream>

#include "Error.hpp"

class Serializer {
    static constexpr char Separator = ' ';

   public:
    explicit Serializer(std::ostream& out) : out_(out) {}

    template <class T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args) {
        return process(args...);
    }

   private:
    std::ostream& out_;

    template <class T, class... Args>
    Error process(const T& value, const Args&... args) {
        // на самом деле, Error::CorruptedArchive никогда не придет
        Error err = process(value);
        // проверка на возвращаемое значение
        if (err == Error::CorruptedArchive) {
            return err;
        }

        // рекурсивно запускаем для остальных аргументов
        return process(args...);
    }

    Error process(const bool& arg) {
        // в зависимости от полученного значения bool
        // возвращаем строку true или false
        out_ << (arg ? "true" : "false") << Separator;

        return Error::NoError;
    }

    Error process(const uint64_t& arg) {
        // просто выводим полученное число
        // ошибок при касте к строке никогда не будет
        out_ << arg << Separator;

        return Error::NoError;
    }
};