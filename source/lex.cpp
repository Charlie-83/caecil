#include "lex.hpp"
#include <algorithm>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<Token> getTokens() {
    std::vector<Token> tokens = {};
    std::string accumulator = "";
    AccumulatorState accumulator_state = AccumulatorState::none;
    while (true) {
        char character = getchar();

        std::vector<char> terminal_characters = {' ', EOF, '\n', '(', ')', '{', '}',
                                                 '[', ']', ',',  '=', ':', ';'};
        if (std::find(terminal_characters.begin(), terminal_characters.end(),
                      character) != terminal_characters.end()) {
            switch (accumulator_state) {
            case AccumulatorState::string:
                if (accumulator == "")
                    break;
                else if (accumulator == "function") {
                    tokens.emplace_back(TokenType::function, nullptr);
                    break;
                } else if (accumulator == "string") {
                    tokens.emplace_back(TokenType::type, DataType::string);
                    break;
                } else if (accumulator == "number") {
                    tokens.emplace_back(TokenType::type, DataType::number);
                    break;
                }
                tokens.emplace_back(TokenType::id, accumulator);
                break;
            case AccumulatorState::number:
                tokens.emplace_back(TokenType::number, strtod(accumulator.c_str(), 0));
                break;
            case AccumulatorState::none:
                break;
            }
            switch (character) {
            case EOF:
                return tokens;
            case '(':
                tokens.emplace_back(TokenType::open_parenthesis, nullptr);
                break;
            case ')':
                tokens.emplace_back(TokenType::close_parenthesis, nullptr);
                break;
            case '{':
                tokens.emplace_back(TokenType::open_brace, nullptr);
                break;
            case '}':
                tokens.emplace_back(TokenType::close_brace, nullptr);
                break;
            case '[':
                tokens.emplace_back(TokenType::open_bracket, nullptr);
                break;
            case ']':
                tokens.emplace_back(TokenType::close_bracket, nullptr);
                break;
            case ',':
                tokens.emplace_back(TokenType::comma, nullptr);
                break;
            case '=':
                tokens.emplace_back(TokenType::equals, nullptr);
                break;
            case ':':
                tokens.emplace_back(TokenType::colon, nullptr);
                break;
            case ';':
                tokens.emplace_back(TokenType::semicolon, nullptr);
                break;
            }

            accumulator_state = AccumulatorState::none;
            accumulator = "";
        } else {
            accumulator += character;

            if (accumulator_state == AccumulatorState::none) {
                if (isalpha(character) || character == '_')
                    accumulator_state = AccumulatorState::string;
                else if (isdigit(character))
                    accumulator_state = AccumulatorState::number;
                else
                    throw std::logic_error(accumulator);
            }
            if (accumulator_state == AccumulatorState::number && isalpha(character))
                throw std::logic_error(accumulator);
        }
    }
}
