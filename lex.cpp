#include <cctype>
#include <cstdio>
#include <string>

int main()
{
}

union TokenData {
    std::string id;
    double number;
};

enum TokenType {
    tok_id = -1,
    tok_number = -2,
    tok_function = -3,
};

struct Token {
    int type;
    TokenData data;
};

static Token getTok()
{
    char last_character = ' ';

    while(isspace(last_character))
        last_character = getchar();

    if (isalpha(last_character))
    {
        std::string string = "";
        string += last_character;
        while (isalpha(last_character = getchar()))
            string += last_character;
        TokenData data = {string};
        return Token {-1, {string}};
    }
}
