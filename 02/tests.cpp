#include "TokenParser.hpp"
#include "gtest/gtest.h"

TEST(ParserTest, SimpleParserTest) {
    TokenParser parser;
    // В коллбеках просто будет делать вывод в консоль
    parser.SetStartCallback(
        []() { std::cout << "Parsing started" << std::endl; });
    parser.SetEndCallback([]() { std::cout << "Parsing ended" << std::endl; });
    parser.SetDigitTokenCallback([](uint64_t token) {
        std::cout << "Digit token: " << token << std::endl;
    });
    parser.SetStringTokenCallback([](const std::string& token) {
        std::cout << "String token: " << token << std::endl;
    });

    // Захватываем stdout
    ::testing::internal::CaptureStdout();
    parser.Parse("qwer34yu3 134\nklkre\t 42");
    // Получаем захваченные из потока данные
    // Туда должны выводиться соответствующие принты из коллбеков
    std::string output = ::testing::internal::GetCapturedStdout();
    std::string expected =
        "Parsing started\nString token: qwer34yu3\nDigit token: 134\nString "
        "token: klkre\nDigit "
        "token: 42\nParsing ended\n";
    ASSERT_EQ(output, expected);
}

TEST(ParserTest, DefaultCallbacksTest) {
    TokenParser parser;
    // Проверяем, что будет если не задать коллбеки старта и конца
    parser.SetDigitTokenCallback([](uint64_t token) {
        std::cout << "Digit token: " << token << std::endl;
    });
    parser.SetStringTokenCallback([](const std::string& token) {
        std::cout << "String token: " << token << std::endl;
    });

    // Захватываем stdout
    ::testing::internal::CaptureStdout();
    parser.Parse(" 123 qwerty");
    // Получаем захваченные из потока данные
    // Туда должны выводиться соответствующие принты из коллбеков
    std::string output = ::testing::internal::GetCapturedStdout();
    // Ожидается, что будет только вывод из переопределенных коллбеков
    // А коллбеки начала и конца будут по умолчанию ничего не делать
    std::string expected = "Digit token: 123\nString token: qwerty\n";
    ASSERT_EQ(output, expected);
}

TEST(ParserTest, DefaultTokenCallbacksTest) {
    TokenParser parser;
    // Проверяем, что будет если не задать коллбеки для обработки строк и чисел
    parser.SetStartCallback(
        []() { std::cout << "Parsing started" << std::endl; });
    parser.SetEndCallback([]() { std::cout << "Parsing ended" << std::endl; });

    // Захватываем stdout
    ::testing::internal::CaptureStdout();
    parser.Parse(" 123 qwerty");
    // Получаем захваченные из потока данные
    // Туда должны выводиться соответствующие принты из коллбеков
    std::string output = ::testing::internal::GetCapturedStdout();
    // Ожидается, что будет только вывод из переопределенных коллбеков
    // А коллбеки для строк и чисел будут по умолчанию ничего не делать
    std::string expected = "Parsing started\nParsing ended\n";
    ASSERT_EQ(output, expected);
}

TEST(ParserTest, BlankLinesTest) {
    TokenParser parser;
    parser.SetStartCallback(
        []() { std::cout << "Parsing started" << std::endl; });
    parser.SetEndCallback([]() { std::cout << "Parsing ended" << std::endl; });
    parser.SetDigitTokenCallback([](uint64_t token) {
        std::cout << "Digit token: " << token << std::endl;
    });
    parser.SetStringTokenCallback([](const std::string& token) {
        std::cout << "String token: " << token << std::endl;
    });

    // Захватываем stdout
    ::testing::internal::CaptureStdout();
    // Пробуем обработать пустую строку
    parser.Parse("");
    std::string output = ::testing::internal::GetCapturedStdout();
    // Ожидается, что ничего не упадет
    // Коллбеки ни для строки, ни для числа вызваны не будут
    std::string expected = "Parsing started\nParsing ended\n";
    ASSERT_EQ(output, expected);

    // Также выполним проверку для символов-разделителей
    ::testing::internal::CaptureStdout();
    parser.Parse(" ");
    output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, expected);

    ::testing::internal::CaptureStdout();
    parser.Parse(" \n\t   ");
    output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, expected);
}

TEST(ParserTest, OverflowDigitTest) {
    TokenParser parser;
    parser.SetStartCallback(
        []() { std::cout << "Parsing started" << std::endl; });
    parser.SetEndCallback([]() { std::cout << "Parsing ended" << std::endl; });
    parser.SetDigitTokenCallback([](uint64_t token) {
        std::cout << "Digit token: " << token << std::endl;
    });
    parser.SetStringTokenCallback([](const std::string& token) {
        std::cout << "String token: " << token << std::endl;
    });

    ::testing::internal::CaptureStdout();
    // Пробуем спарисить большое число, но еще не превышающее uint64_t
    parser.Parse("18446744073709551615");
    std::string output = ::testing::internal::GetCapturedStdout();
    // Ожидается, что вызовется digit_callback_
    std::string expected =
        "Parsing started\nDigit token: 18446744073709551615\nParsing ended\n";
    ASSERT_EQ(output, expected);

    ::testing::internal::CaptureStdout();
    // Теперь передаем уже число, не влезающее в uint64_t
    parser.Parse("18446744073709551616");
    output = ::testing::internal::GetCapturedStdout();
    // Ожидается, что тут уже вызовется string_callback_
    expected =
        "Parsing started\nString token: 18446744073709551616\nParsing ended\n";
    ASSERT_EQ(output, expected);
}

TEST(ParserTest, SingleTokenTest) {
    TokenParser parser;
    parser.SetStartCallback(
        []() { std::cout << "Parsing started" << std::endl; });
    parser.SetEndCallback([]() { std::cout << "Parsing ended" << std::endl; });
    parser.SetDigitTokenCallback([](uint64_t token) {
        std::cout << "Digit token: " << token << std::endl;
    });
    parser.SetStringTokenCallback([](const std::string& token) {
        std::cout << "String token: " << token << std::endl;
    });

    // Проверка корректной обработки одного токена (для строки и числа)
    ::testing::internal::CaptureStdout();
    parser.Parse("token");
    std::string output = ::testing::internal::GetCapturedStdout();
    std::string expected =
        "Parsing started\nString token: token\nParsing ended\n";
    ASSERT_EQ(output, expected);

    ::testing::internal::CaptureStdout();
    parser.Parse("0042");
    output = ::testing::internal::GetCapturedStdout();
    expected = "Parsing started\nDigit token: 42\nParsing ended\n";
    ASSERT_EQ(output, expected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}