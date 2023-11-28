#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

#include <iostream>

class Myexception
{
    public:
        Myexception();
        virtual std::string what();
};

class StatementError : public Myexception
{
    private:
        std::string sta;
    public:
        StatementError(std::string t);
        virtual std::string what() override;
};

class IdentifierError : public Myexception
{
    private:
        std::string errorID;
    public:
        IdentifierError(std::string e);
        virtual std::string what() override;
};

class ExpressionError : public Myexception
{
    private:
        std::string exp;
    public:
        ExpressionError(std::string e);
        virtual std::string what() override;
};

class NameError : public Myexception
{
    private:
        std::string name;
    public:
        NameError(std::string n);
        virtual std::string what() override;
};

class LineError : public Myexception
{
    private:
        std::string lineNumber;
    public:
        LineError(std::string n);
        virtual std::string what() override;
};

class InputError : public Myexception
{
    private:
        std::string input;
    public:
        InputError(std::string n);
        virtual std::string what() override;
};

#endif // MYEXCEPTION_H
