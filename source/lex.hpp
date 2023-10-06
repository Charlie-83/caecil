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

struct Node
{
};

enum class Type
{
    string,
    number
};

enum class BinaryOp
{
    plus,
};

struct PrototypeNode : Node
{
    std::string name;
    std::vector<Type> types;
    std::vector<std::string> args;
};

struct FunctionNode : Node
{
    PrototypeNode prototype;
};

struct ExpressionNode : Node
{
};

struct BinaryOpNode : Node
{
    ExpressionNode left;
    BinaryOp operation;
    ExpressionNode right;
};

struct VariableNode : Node
{
    std::string name;
};

struct NumberNode : Node
{
    double value;
};

struct SequenceNode : Node
{
    std::vector<Node> nodes;
};

std::vector<Token> getTokens();
Node getAST(std::vector<Token> tokens);
