#pragma once
#include "lex.hpp"
#include <string>
#include <vector>

struct Node {};

enum class BinaryOp {
    plus,
};

struct PrototypeNode : Node {
    DataType return_type;
    std::string name;
    std::vector<DataType> types;
    std::vector<std::string> args;
};

struct FunctionNode : Node {
    PrototypeNode prototype;
    Node body;
};

struct ExpressionNode : Node {};

struct BinaryOpNode : Node {
    ExpressionNode left;
    BinaryOp operation;
    ExpressionNode right;
};

struct VariableNode : Node {
    std::string name;
};

struct AssignmentNode : Node {
    VariableNode variable;
    Node rhs;
};

struct DeclareAssignmentNode : Node {
    DataType lhs_type;
    VariableNode variable;
    Node rhs;
};

struct NumberNode : Node {
    double value;
};

struct SequenceNode : Node {
    std::vector<Node> nodes;
};

struct EmptyNode : Node {};

Node getAST(std::vector<Token> tokens);
Node parse(std::vector<Token> tokens, int &pointer);
Node parseNumber(std::vector<Token> tokens, int &pointer);
Node parseExpression(std::vector<Token> tokens, int &pointer);
Node parseFunction(std::vector<Token> tokens, int &pointer);
Node parseAssign(std::vector<Token> tokens, int &pointer);
Node parseDeclareAssign(std::vector<Token> tokens, int &pointer);

void printTree(Node root);
