#include "parse.hpp"
#include "lex.hpp"
#include <stdexcept>

static Token check(int pointer, std::vector<Token> tokens, TokenType type) {
    if (tokens[pointer].type != type) {
        std::string str;
        switch (type) {
        case (TokenType::type):
            str = "type";
            break;
        case (TokenType::id):
            str = "id";
            break;
        case (TokenType::eof):
            str = "eof";
            break;
        case (TokenType::semicolon):
            str = ";";
            break;
        case (TokenType::colon):
            str = ":";
            break;
        case (TokenType::comma):
            str = ",";
            break;
        case (TokenType::equals):
            str = "=";
            break;
        case (TokenType::open_brace):
            str = "{";
            break;
        case (TokenType::close_brace):
            str = "}";
            break;
        case (TokenType::open_bracket):
            str = "[";
            break;
        case (TokenType::close_bracket):
            str = "]";
            break;
        case (TokenType::open_parenthesis):
            str = "(";
            break;
        case (TokenType::close_parenthesis):
            str = ")";
            break;
        case (TokenType::number):
            str = "number";
            break;
        case (TokenType::function):
            str = "function";
            break;
        }
        std::logic_error("Expected: " + str);
    }
    return tokens[pointer];
}

template <typename T>
static T checkGetData(int pointer, std::vector<Token> tokens, TokenType type) {
    return std::get<T>(check(pointer, tokens, type).data);
}

Node getAST(std::vector<Token> tokens) {
    int pointer = 0;
    return parse(tokens, pointer);
}

Node parse(std::vector<Token> tokens, int &pointer) {
    SequenceNode expression_sequence = {{}, {}};
    while (pointer < (int)tokens.size()) {
        expression_sequence.nodes.push_back(parseExpression(tokens, pointer));
        pointer++;
    }
    return expression_sequence;
}

Node parseExpression(std::vector<Token> tokens, int &pointer) {
    switch (tokens[pointer].type) {
    case TokenType::function:
        pointer++;
        return parseFunction(tokens, pointer);
    case TokenType::type:
        return parseDeclareAssign(tokens, pointer);
    case TokenType::id:
        return parseDeclareAssign(tokens, pointer);
    case TokenType::number:
        return parseNumber(tokens, pointer);
    default:
        throw std::logic_error("Wrong token at start of expression");
    }
}

Node parseNumber(std::vector<Token> tokens, int &pointer) {
    double value = checkGetData<double>(pointer, tokens, TokenType::number);
    ++pointer;
    return NumberNode{{}, value};
}

Node parseFunction(std::vector<Token> tokens, int &pointer) {
    DataType return_type;
    std::string name;
    return_type = checkGetData<DataType>(pointer, tokens, TokenType::type);
    name = checkGetData<std::string>(pointer + 1, tokens, TokenType::id);
    check(pointer + 1, tokens, TokenType::open_parenthesis);

    pointer += 3;
    std::vector<DataType> types = {};
    std::vector<std::string> args = {};
    while (tokens[pointer].type !=
           TokenType::close_parenthesis) // TODO: Deal with nested parenthesis
    {
        types.push_back(checkGetData<DataType>(pointer, tokens, TokenType::type));
        args.push_back(checkGetData<std::string>(pointer + 1, tokens, TokenType::id));
        check(pointer + 2, tokens, TokenType::comma);
        pointer += 3;
    }
    pointer += 1;
    if (tokens[pointer].type != TokenType::open_brace)
        return PrototypeNode{{}, return_type, name, types, args};
    else {
        pointer += 1;
        std::vector<Node> body = {};
        while (tokens[pointer].type != TokenType::close_brace) {
            body.push_back(parseExpression(tokens, pointer));
        }
        switch (body.size()) {
        case 0:
            return FunctionNode{
                {}, PrototypeNode{{}, return_type, name, types, args}, EmptyNode()};
        case 1:
            return FunctionNode{
                {}, PrototypeNode{{}, return_type, name, types, args}, body[0]};
        default:
            return FunctionNode{{},
                                PrototypeNode{{}, return_type, name, types, args},
                                SequenceNode{{}, body}};
        }
    }
}

Node parseAssign(std::vector<Token> tokens, int &pointer) {
    auto name = checkGetData<std::string>(pointer + 1, tokens, TokenType::id);
    check(pointer + 2, tokens, TokenType::equals);
    pointer += 3;
    Node rhs = parseExpression(tokens, pointer);
    return AssignmentNode{{}, {{}, name}, rhs};
}

Node parseDeclareAssign(std::vector<Token> tokens, int &pointer) {
    auto type = checkGetData<DataType>(pointer, tokens, TokenType::type);
    auto name = checkGetData<std::string>(pointer + 1, tokens, TokenType::id);
    check(pointer + 2, tokens, TokenType::equals);
    pointer += 3;
    Node rhs = parseExpression(tokens, pointer);
    return DeclareAssignmentNode{{}, type, {{}, name}, rhs};
}
