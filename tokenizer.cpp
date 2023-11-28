#include "tokenizer.h"
#include "myexception.h"

#include<sstream>

Tokenizer::Tokenizer() { }
Tokenizer::~Tokenizer() { }

std::string Tokenizer::char2string(char c)
{
    return std::string(1, c);
}

std::string Tokenizer::char2string(char c1, char c2)
{
    return std::string(1, c1) + std::string(1, c2);
}

Token Tokenizer::string2tokens(std::string cmd)
{
    tokens.clear();
    std::stringstream s(cmd);
    std::string block;
    std::string buf;
    bool constant = false;

    while (s >> block) {
        buf.clear();
        for (size_t i = 0 ; i < block.length(); i++) {
            // ** is a special operator
            if (i + 1 < block.length() && operators.count(char2string(block[i], block[i + 1]))) {
                if (!buf.empty()) {
                    tokens.push_back(buf);
                    buf.clear();
                }
                tokens.push_back(char2string(block[i], block[i + 1]));
                i++;
                continue;
            }

            // then check if other types of operators exist
            if (operators.count(char2string(block[i]))) {
                if (!buf.empty()) {
                    tokens.push_back(buf);
                    buf.clear();
                }
                tokens.push_back(char2string(block[i]));
                continue;
            }

            // parsing numbers and letters
            if (buf.empty()) {
                constant = isDigit(block[i]);
                buf.push_back(block[i]);
                continue;
            }
            if (constant) {
                if (isDigit(block[i])) {
                    buf.push_back(block[i]);
                } else {
                    throw IdentifierError(char2string(buf.back(), block[i]));
                }
            } else {
                buf.push_back(block[i]);
            }
        }

        if (!buf.empty())
            tokens.push_back(buf);
    }

    return tokens;
}

std::string Tokenizer::tokens2string(Token token)
{
    std::string res = "";
    for (auto & t : token) {
        res += t;
        res += " ";
    }
    res.pop_back();  // eliminate the last space in the string
    return res;
}

bool Tokenizer::isDigit(char c) const
{
    return c >= '0' && c <= '9';
}
