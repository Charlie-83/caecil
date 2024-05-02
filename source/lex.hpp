#pragma once
#include <fmt/core.h>
#include <string>
#include <variant>
#include <vector>

enum class TokenType {
    id,
    number,
    function,
    eof,
    type,
    open_parenthesis,
    close_parenthesis,
    open_bracket,
    close_bracket,
    open_brace,
    close_brace,
    comma,
    equals,
    semicolon,
    colon,
};

enum class DataType { string, number };

struct Token {
    TokenType type;
    std::variant<std::string, double, DataType, std::nullptr_t> data;
};

enum class AccumulatorState {
    none,
    string,
    number,
};

std::vector<Token> getTokens();
