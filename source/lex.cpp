#include <lex.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <variant>
#include <vector>

std::vector<Token> getTokens()
{
    std::vector<Token> tokens = {};
    std::string accumulator = "";
    AccumulatorState accumulator_state = AccumulatorState::none;
    while (true)
    {
        char character = getchar();
        if (character == ' ' || character == EOF)
        {
            switch (accumulator_state)
            {
            case AccumulatorState::string:
                if (accumulator == "function")
                {

                    tokens.emplace_back(TokenType::function, nullptr);
                    break;
                }
                tokens.emplace_back(TokenType::id, accumulator);
                break;
            case AccumulatorState::number:
                tokens.emplace_back(TokenType::number, strtod(accumulator.c_str(), 0));
                break;
            case AccumulatorState::none:
                break;
            case AccumulatorState::unknown:
                tokens.emplace_back(TokenType::other, accumulator);
                break;
            }

            if (character == EOF)
            {
                return tokens;
            }

            accumulator_state = AccumulatorState::none;
            accumulator = "";
        }
        else
        {
            accumulator += character;

            if (accumulator_state == AccumulatorState::none)
            {
                if (isalpha(character))
                    accumulator_state = AccumulatorState::string;
                else if (isdigit(character))
                    accumulator_state = AccumulatorState::number;
                else
                    accumulator_state = AccumulatorState::unknown;
            }
            if (accumulator_state == AccumulatorState::number && isalpha(character))
                accumulator_state = AccumulatorState::unknown;
        }
    }
}
