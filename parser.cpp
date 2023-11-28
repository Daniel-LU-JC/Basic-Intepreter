#include "parser.h"
#include "myexception.h"

#include <sstream>

using namespace myExpr;

Parser::Parser() { }
Parser::~Parser() { }

void Parser::merge() {
    if (operators.empty())
        throw Myexception();
    std::string op = operators.top();
    operators.pop();

    if (operands.empty())
        throw Myexception();

    Expression * right = operands.top();
    operands.pop();
    bool negative = false;
    if (op == "-") {
        negative = isNegative.top();
        isNegative.pop();
    }

    Expression *left = nullptr;
    if (!negative) {
        if (operands.empty())
            throw Myexception();
        left = operands.top();
        operands.pop();
    }

    // merge left and right expression into a compound expression
    operands.push(new CompoundExp(op,left,right));
}

Expression * Parser::token2Exp(Token tokens) {
    while (!operands.empty()){
        operands.pop();
    }
    while(!operators.empty()){
        operators.pop();
    }  // Initialization

    isFirst = true;

    for (auto & token : tokens) {
        if (validOperators.count(token)) {
            if (token == "(") {
                operators.push(token);
                isFirst = true;
                continue;
            }
            if (token == ")") {
                while (operators.top() != "(")
                    merge();
                operators.pop();
                continue;
            }

            while (!operators.empty() && precedence[operators.top()] >= precedence[token])
                merge();

            operators.push(token);

            if (token == "-") {
                if (isFirst) {
                    isNegative.push(true);
                } else {
                    isNegative.push(false);
                }
            }

            isFirst = false;
        } else {  // constant expression or identifier expression
            isFirst = false;
            Expression *exp = nullptr;
            if (isConstant(token)) {
                exp = new ConstantExp(getConstant(token));
            } else {
                exp = new IdentifierExp(token);
            }
            operands.push(exp);
        }
    }

    while (!operators.empty())
        merge();

    if (operands.empty() || operands.size() > 1)
        throw Myexception();

    return operands.top();
}

bool Parser::isConstant(std::string s) const
{
    for (auto c : s)
        if (c < '0' || c > '9')
            return false;
    return true;
}

int Parser::getConstant(std::string s)
{
    std::stringstream ss(s);
    int res;
    ss >> res;
    return res;
}
