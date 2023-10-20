#pragma once
#include <fmt/core.h>
#include <string>
#include <variant>
#include <vector>

union TokenData
{
    std::string id;
    double number;
};

enum class TokenType
{
    id,
    number,
    function,
    eof,
    type,
    newline,
    open_parenthesis,
    close_parenthesis,
    open_bracket,
    close_bracket,
    open_brace,
    close_brace,
    comma,
    equals,
};

enum class DataType
{
    string,
    number
};

struct Token
{
    TokenType type;
    std::variant<std::string, double, DataType, std::nullptr_t> data;
};

enum class AccumulatorState
{
    none,
    string,
    number,
};

std::vector<Token> getTokens();
