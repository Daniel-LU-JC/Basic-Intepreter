#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stack>
#include <set>
#include <iostream>
#include <expression.h>
#include <map>

using namespace myExpr;
using Token = std::vector<std::string>;

class Parser
{
    public:
        Parser();
        ~Parser();
        Expression * token2Exp(Token token);

    private:
        void merge();
        bool isConstant(std::string s) const;
        int getConstant(std::string s);
        std::stack<std::string> operators;
        std::stack<Expression *> operands;

        bool isFirst;
        std::stack<bool> isNegative;
};

#endif // PARSER_H
