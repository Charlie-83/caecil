#include "parse.hpp"
#include "lex.hpp"
#include <cstddef>
#include <stdexcept>

Node getAST(std::vector<Token> tokens) { return parse(tokens, 0); }

Node parse(std::vector<Token> tokens, int *pointer)
{
    SequenceNode expression_sequence = {{}, {}};
    while (*pointer < (int)tokens.size())
    {
        expression_sequence.nodes.push_back(parseExpression(tokens, pointer));
        (*pointer)++;
    }
    return expression_sequence;
}

Node parseExpression(std::vector<Token> tokens, int *pointer)
{
    switch (tokens[*pointer].type)
    {
    case TokenType::function:
        (*pointer)++;
        return parseFunction(tokens, pointer);
    case TokenType::type:
        return parseAssignment(tokens, pointer);
    }
}

Node parseFunction(std::vector<Token> tokens, int *pointer)
{
    DataType return_type;
    std::string name;
    if (tokens[*pointer].type != TokenType::type)
        throw std::invalid_argument("No return type specified for function");
    else
        return_type = std::get<DataType>(tokens[*pointer].data);
    if (tokens[*pointer + 1].type != TokenType::id)
        throw std::invalid_argument("Expected id after 'function'");
    else
        name = std::get<std::string>(tokens[*pointer + 1].data);
    if (tokens[*pointer + 2].type != TokenType::open_parenthesis)
        throw std::invalid_argument("Expected bracket");
    *pointer += 3;
    std::vector<DataType> types = {};
    std::vector<std::string> args = {};
    while (tokens[*pointer].type !=
           TokenType::close_parenthesis) // TODO: Deal with nested parenthesis
    {
        if (tokens[*pointer].type == TokenType::type)
            types.push_back(std::get<DataType>(tokens[*pointer].data));
        else
            throw std::invalid_argument("Expected type in function prototype");

        if (tokens[*pointer + 1].type == TokenType::id)
            args.push_back(std::get<std::string>(tokens[*pointer + 1].data));
        else
            throw std::invalid_argument("Expected name for function parameter");

        if (tokens[*pointer + 2].type != TokenType::comma)
            throw std::invalid_argument("Expected comma in function prototype");
        *pointer += 3;
    }
    *pointer += 1;
    if (tokens[*pointer].type != TokenType::open_brace)
        return PrototypeNode{{}, return_type, name, types, args};
    else
    {
        *pointer += 1;
        std::vector<Node> body = {};
        while (tokens[*pointer].type != TokenType::close_brace)
        {
            body.push_back(parseExpression(tokens, pointer));
        }
        switch (body.size())
        {
        case 0:
            return FunctionNode{{}, PrototypeNode{{}, return_type, name, types, args}, EmptyNode()};
        case 1:
            return FunctionNode{{}, PrototypeNode{{}, return_type, name, types, args}, body[0]};
        default:
            return FunctionNode{
                {}, PrototypeNode{{}, return_type, name, types, args}, SequenceNode{{}, body}};
        }
    }
}

Node parseAssignment(std::vector<Token> tokens, int *pointer)
{
    DataType type = std::get<DataType>(tokens[*pointer].data);
    if (tokens[*pointer + 1].type != TokenType::id)
        throw std::logic_error("Expected id");
    if (tokens[*pointer + 2].type != TokenType::equals)
        throw std::logic_error("Expected =");
    std::string name = std::get<std::string>(tokens[*pointer + 1].data);
    *pointer += 3;
    Node rhs = parseExpression(tokens, pointer);
    return AssignmentNode{{}, type, {{}, name}, rhs};
}
