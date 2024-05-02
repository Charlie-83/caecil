#include <cstdio>
#include <iostream>
#include <lex.hpp>
#include <parse.hpp>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <vector>

int main() {
    spdlog::set_level(spdlog::level::debug);
    freopen("source/test.cae", "r", stdin);
    std::vector<Token> tokens = getTokens();

    std::cout << std::format("Found {} tokens", tokens.size()) << std::endl;

    for (Token t : tokens) {
        switch (t.type) {
        case (TokenType::id):
            std::cout << "id: " + std::get<std::string>(t.data) << std::endl;
            break;
        case (TokenType::number):
            std::cout << std::format("number: {}", std::get<double>(t.data)) << std::endl;
            break;
        case (TokenType::function):
            std::cout << "function" << std::endl;
            break;
        case (TokenType::type):
            switch (std::get<DataType>(t.data)) {
            case (DataType::number):
                std::cout << "number type" << std::endl;
                break;
            case (DataType::string):
                std::cout << "string type" << std::endl;
                break;
            }
            break;
        case (TokenType::eof):
            std::cout << "eof" << std::endl;
            break;
        default:
            std::cout << "other" << std::endl;
            break;
        }
    }

    int pointer = 0;
    auto root = parse(tokens, pointer);
}
