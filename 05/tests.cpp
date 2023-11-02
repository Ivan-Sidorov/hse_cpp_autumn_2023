#include <iostream>
#include <sstream>
#include <string>

#include "Deserializer.hpp"
#include "Error.hpp"
#include "Serializer.hpp"
#include "gtest/gtest.h"

struct DataA {
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a, b, c);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer) {
        return deserializer(a, b, c);
    }
};

struct DataB {
    bool a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a, b, c);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer) {
        return deserializer(a, b, c);
    }
};

struct DataC {
    bool a;

    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer) {
        return deserializer(a);
    }
};

struct DataD {
    uint64_t a;

    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer) {
        return deserializer(a);
    }
};

TEST(SerializerTest, SimpleTest) {
    DataA x{3, true, 2};

    std::stringstream _stream;
    Serializer serializer(_stream);
    Error err_ser = serializer.save(x);

    DataA y{0, false, 0};

    Deserializer deserializer(_stream);
    Error err_des = deserializer.load(y);

    // ожидается, что все прошло без ошибок
    ASSERT_EQ(err_ser, Error::NoError);
    ASSERT_EQ(err_des, Error::NoError);

    // ожидается, что после сериализации и десериализации
    // получатся те же значения
    ASSERT_EQ(x.a, y.a);
    ASSERT_EQ(x.b, y.b);
    ASSERT_EQ(x.c, y.c);
}

TEST(SerializerTest, DiffTypesTest) {
    DataA x{3, true, 2};

    std::stringstream _stream;
    Serializer serializer(_stream);
    Error err_ser = serializer.save(x);

    DataB y{false, false, 0};

    Deserializer deserializer(_stream);
    Error err_des = deserializer.load(y);

    // ожидается, что сериализация прошла без ошибок,
    // при десериализации возникло исключение
    ASSERT_EQ(err_ser, Error::NoError);
    ASSERT_EQ(err_des, Error::CorruptedArchive);

    // ожидается, что после десериализации
    // значения остались теми же из-за исключения
    ASSERT_EQ(y.a, false);
    ASSERT_EQ(y.b, false);
    ASSERT_EQ(y.c, 0);
}

TEST(SerializerTest, SingleValueBoolTest) {
    DataC x_in{true};

    std::stringstream _stream;
    Serializer serializer(_stream);
    Error err_ser = serializer.save(x_in);

    DataC x_out{false};

    Deserializer deserializer(_stream);
    Error err_des = deserializer.load(x_out);

    // ожидается, что все прошло без ошибок
    ASSERT_EQ(err_ser, Error::NoError);
    ASSERT_EQ(err_des, Error::NoError);

    // ожидается, что после сериализации и десериализации
    // получится то же значение
    ASSERT_EQ(x_in.a, x_out.a);
}

TEST(SerializerTest, SingleValueIntTest) {
    DataD x_in{5};

    std::stringstream _stream;
    Serializer serializer(_stream);
    Error err_ser = serializer.save(x_in);

    DataD x_out{1};

    Deserializer deserializer(_stream);
    Error err_des = deserializer.load(x_out);

    // ожидается, что все прошло без ошибок
    ASSERT_EQ(err_ser, Error::NoError);
    ASSERT_EQ(err_des, Error::NoError);

    // ожидается, что после сериализации и десериализации
    // получится то же значение
    ASSERT_EQ(x_in.a, x_out.a);
}

TEST(SerializerTest, InvalidStringTest) {
    std::stringstream _stream;
    _stream << "invalid_string";

    DataC x{true};
    DataD y{1};

    Deserializer deserializer(_stream);
    Error err_bool = deserializer.load(x);
    Error err_uint = deserializer.load(y);

    // ожидается, что сериализация в обоих случаях
    // пройдет с ошибкой, так как строку нельзя привести
    // ни к bool, ни к uint64_t
    ASSERT_EQ(err_bool, Error::CorruptedArchive);
    ASSERT_EQ(err_uint, Error::CorruptedArchive);
}

TEST(SerializerTest, IntOverflowTest) {
    std::stringstream _stream;
    _stream << "42 false 123456789012345678901234567890";

    DataA x{1, true, 2};

    Deserializer deserializer(_stream);
    Error err_des = deserializer.load(x);

    // так как число превышает возможно допустимое для uint64_t,
    // ожидается ошибка Error::CorruptedArchive
    ASSERT_EQ(err_des, Error::CorruptedArchive);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}