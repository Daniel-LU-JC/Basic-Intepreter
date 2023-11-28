#ifndef TOKENIZER_H
#define TOKENIZER_H

#include<vector>
#include<iostream>
#include<set>

using Token = std::vector<std::string>;

class Tokenizer
{
    std::set<std::string> operators =
        {"+", "-", "*", "/", "**", "=", "<", ">", "(", ")"};

private:
    Token tokens;
    bool isDigit(char c) const;
    std::string char2string(char c);
    std::string char2string(char c1, char c2);

public:
    Tokenizer();
    ~Tokenizer();
    Token string2tokens(std::string s);
    std::string tokens2string(Token token);
};

#endif // TOKENIZER_H
