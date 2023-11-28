#include "statement.h"

#include <queue>

using myExpr::Expression;
using myExpr::pad;

std::string Statement::itos(int n)
{
    std::stringstream ss;
    ss << n;
    std::string res;
    ss >> res;
    return res;
}  // from int to string

Statement::Statement() { }

Tree Statement::getTree() const
{
    return tree;
}

Expression * Statement::getExp() const
{
    return nullptr;
}

Statement::~Statement() { }

LETsta::LETsta(std::string var, Expression *exp) : var(var), exp(exp)
{
    createTree();
}

LETsta::~LETsta()
{
    delete exp;
}

Expression * LETsta::getExp() const
{
    return exp;
}

void LETsta::exec(Evalstate *state)  // execute the expression
{
    state->setNext();
    state->setValue(var, exp->eval(state));
}

GOTOsta::GOTOsta(int n) : lineNumber(n)
{
    createTree();
}

GOTOsta::~GOTOsta() { }

void GOTOsta::exec(Evalstate *state)
{
    state->setGoto(lineNumber);
}

PRINTsta::PRINTsta(Expression *exp) : exp(exp)
{
    createTree();
}

PRINTsta::~PRINTsta()
{
    delete exp;
}

Expression * PRINTsta::getExp() const
{
    return exp;
}

void PRINTsta::exec(Evalstate *state)
{
    state->setNext();
    int res = exp->eval(state);
    state->print(itos(res));  // print the result of expression when executing
}

IFsta::IFsta(std::string op, Expression *l, Expression *r, int n)
    : op(op), left(l), right(r), lineNumber(n)
{
    createTree();
}

IFsta::~IFsta()
{
    delete left;
    delete right;
}

void IFsta::exec(Evalstate *state)
{
    state->setNext();
    int l = left->eval(state);
    int r = right->eval(state);  // get the value of left and right expression
    bool go = false;
    switch (op[0]) {
        case '>':
            go = l > r;
            break;
        case '<':
            go = l < r;
            break;
        case '=':
            go = l == r;
            break;
    }
    if (go) state->setGoto(lineNumber);  // update the next line number to be executed
}

ENDsta::ENDsta()
{
    createTree();
}

ENDsta::~ENDsta() { }

void ENDsta::exec(Evalstate *state)
{
    state->setEnd();  // terminate the current program
}

REMsta::REMsta(std::string c) : content(c)
{
    createTree();
}

REMsta::~REMsta() { }

void REMsta::exec(Evalstate *state)
{
    state->setNext();  // nothing to be done for comments
}

INPUTsta::INPUTsta(std::string name) : name(name)
{
    createTree();
}

void INPUTsta::exec(Evalstate *state)
{
    state->setNext();
    state->print(name + "=?");
    state->startInput(name);  // request input value of the variable from user
}

INPUTsta::~INPUTsta() { }

void INPUTsta::createTree()
{
    tree.push_back("INPUT");
    tree.push_back(pad + name);
}

void LETsta::createTree()
{
    tree.push_back("LET = ");
    tree.push_back(pad + var);
    for (std::string & line : exp->tree)
        tree.push_back(pad + line);
}

void PRINTsta::createTree()
{
    tree.push_back("PRINT");
    for (std::string & line : exp->tree)
        tree.push_back(pad + line);
}

void REMsta::createTree()
{
    tree.push_back("REM");
    tree.push_back(pad + content);
}

void GOTOsta::createTree()
{
    tree.push_back("GOTO");
    tree.push_back(pad + itos(lineNumber));
}

void ENDsta::createTree()
{
    tree.push_back("END");
}

void IFsta::createTree()
{
    tree.push_back("IF THEN");
    for (std::string & line : left->tree)
        tree.push_back(pad+line);
    tree.push_back(pad + op);
    for (std::string & line : right->tree)
        tree.push_back(pad+line);
    tree.push_back(pad + itos(lineNumber));
}
