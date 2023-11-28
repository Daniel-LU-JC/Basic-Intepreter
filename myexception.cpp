#include "myexception.h"

Myexception::Myexception() { }

std::string Myexception::what()
{
    return "error";
}

StatementError::StatementError(std::string t):sta(t) { }
std::string StatementError::what()
{
    return "statement error: " + sta;
}

IdentifierError::IdentifierError(std::string e):errorID(e) { }
std::string IdentifierError::what()
{
    return "identifier error: " + errorID;
}

ExpressionError::ExpressionError(std::string e):exp(e) { }
std::string ExpressionError::what()
{
    return "expression error: " + exp;
}

NameError::NameError(std::string n):name(n) { }
std::string NameError::what()
{
    return "name error: name '" + name + "' is not defined";
}

LineError::LineError(std::string n):lineNumber(n) { }
std::string LineError::what()
{
    return "line error: line " + lineNumber + " is not defined";
}

InputError::InputError(std::string n):input(n) { }
std::string InputError::what()
{
    return "input error: '" + input + "' is not a number";
}
