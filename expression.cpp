#include "expression.h"
#include "myexception.h"

#include <cmath>
#include <queue>

using myExpr::Expression;
using myExpr::ConstantExp;
using myExpr::IdentifierExp;
using myExpr::CompoundExp;
using myExpr::ExpressionType;

Expression::Expression() { }
Expression::~Expression() { }

std::string Expression::getOperator() const
{
    return "";
}

Expression* Expression::getLeft() const
{
    return nullptr;
}

Expression* Expression::getRight() const
{
    return nullptr;
}

ConstantExp::ConstantExp(int value) : value(value)
{
    tree.push_back(toString());
}
ConstantExp::~ConstantExp() { }

IdentifierExp::IdentifierExp(std::string n)
{
    if (banned.count(n)) {
        throw IdentifierError(n);
    } else {
        name = n;
    }
    tree.push_back(toString());
}
IdentifierExp::~IdentifierExp() { }

CompoundExp::CompoundExp(std::string op, Expression *left, Expression *right)
    : op(op), left(left), right(right)
{
    tree.push_back(op);

    if (left)
        for (std::string & line : left->tree) {
            tree.push_back(pad + line);
        }

    if (right)
        for (std::string & line : right->tree) {
            tree.push_back(pad + line);
        }
}

CompoundExp::~CompoundExp()
{
    delete left;
    delete right;
}

int ConstantExp::eval(Evalstate *) {
   return value;
}

std::string ConstantExp::toString() const
{
    std::stringstream ss;
    ss << value;
    std::string s;
    ss >> s;
    return s;
}  // from int to string

int IdentifierExp::eval(Evalstate *state) {
   return state->getValue(name);
}

std::string IdentifierExp::toString() const
{
    return name;
}

int CompoundExp::eval(Evalstate *state) {
   int l = left ? left->eval(state) : 0;
   int r = right ? right->eval(state) : 0;

   if (op == "+") return l + r;
   if (op == "-") return l - r;
   if (op == "*") return l * r;
   if (op == "/") return l / r;
   if (op == "**") return pow(l, r);

   return 0;
}

std::string CompoundExp::toString() const
{
    return "(" + left->toString() + op + right->toString() + ")";
}

ExpressionType ConstantExp::type() const
{
    return CONSTANT;
}
ExpressionType IdentifierExp::type() const
{
    return IDENTIFIER;
}
ExpressionType CompoundExp::type() const
{
    return COMPOUND;
}
std::string CompoundExp::getOperator() const
{
    return op;
}
Expression* CompoundExp::getLeft() const
{
    return left;
}
Expression* CompoundExp::getRight() const
{
    return right;
}
