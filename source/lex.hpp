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
    other,
    eof,
};

struct Token
{
    TokenType type;
    std::variant<std::string, double, std::nullptr_t> data;
};

enum class AccumulatorState
{
    none,
    string,
    number,
    unknown,
};

std::vector<Token> getTokens();
