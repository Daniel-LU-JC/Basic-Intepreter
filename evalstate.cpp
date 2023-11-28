#include "evalstate.h"
#include "myexception.h"

Evalstate::Evalstate()
{
    number = -1;
}

Evalstate::~Evalstate() { }

int Evalstate::getValue(std::string name)
{
    if (name2value.count(name))
        return name2value[name];
    throw NameError(name);
}

void Evalstate::setValue(std::string name, int value)
{
    name2value[name] = value;
}

void Evalstate::setGoto(int n)
{
    number = n;  // the line number of the next instruction to be implemented
}

int Evalstate::getNextLineNumber() const
{
    return number;
}

void Evalstate::setNext()
{
    number = next;  // enum -1
}

void Evalstate::setEnd()
{
    number = end;  // enum -2
}

void Evalstate::reset()
{
    number = next;
    name2value.clear();
    output.clear();
    inputVar.clear();
}

void Evalstate::print(std::string s)
{
    output.push_back(s);
}

std::vector<std::string> Evalstate::getOutput() const
{
    return output;
}

bool Evalstate::isSuspended() const
{
    return !inputVar.empty();  // getting an input from user
}

void Evalstate::startInput(std::string name)
{
    inputVar = name;
}

void Evalstate::finishInput(int value, bool valid)
{
    if (!valid) {
        inputVar.clear();
        return;
    }
    name2value[inputVar] = value;
    inputVar.clear();
    output.push_back(">>" + std::to_string(value));
}
