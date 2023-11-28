#ifndef STATEMENT_H
#define STATEMENT_H

#include <iostream>
#include <vector>
#include <expression.h>
#include <evalstate.h>

using Token = std::vector<std::string>;
using Tree = std::vector<std::string>;

enum StatementType
    {LET, IF, PRINT, INPUT, GOTO, END, REM};

class Statement
{
    protected:
        static std::string itos(int n);  // from int to string
        Tree tree;

    public:
        Statement();
        virtual ~Statement() = 0;  // should be implemented
        Tree getTree() const;
        virtual void createTree() = 0;  // create the syntax tree structure for each statement
        virtual void exec(Evalstate * state) = 0;
        virtual myExpr::Expression * getExp() const;
};

class LETsta : public Statement
{
    private:
        std::string var;
        myExpr::Expression * exp;
        virtual void createTree() override;

    public:
        LETsta(std::string var, myExpr::Expression * exp);
        virtual ~LETsta();
        virtual myExpr::Expression * getExp() const override;
        virtual void exec(Evalstate * state) override;
};

class PRINTsta : public Statement
{
    private:
        myExpr::Expression * exp;
        virtual void createTree() override;
    public:
        PRINTsta(myExpr::Expression * exp);
        virtual ~PRINTsta();
        virtual myExpr::Expression * getExp() const override;
        virtual void exec(Evalstate * state) override;
};

class GOTOsta : public Statement
{
    private:
        int lineNumber;
        virtual void createTree() override;
    public:
        GOTOsta(int n);
        virtual ~GOTOsta();
        virtual void exec(Evalstate * state) override;
};

class IFsta : public Statement
{
    private:
        std::string op;
        myExpr::Expression *left;
        myExpr::Expression *right;
        int lineNumber;
        virtual void createTree() override;
    public:
        IFsta(std::string op, myExpr::Expression *l, myExpr::Expression *r, int n);
        virtual ~IFsta();
        virtual void exec(Evalstate *state) override;
};

class ENDsta : public Statement
{
    private:
        virtual void createTree() override;
    public:
        ENDsta();
        virtual ~ENDsta();
        virtual void exec(Evalstate *state) override;
};

class REMsta : public Statement
{
    private:
        std::string content;
        virtual void createTree() override;
    public:
        REMsta(std::string c);
        virtual ~REMsta();
        virtual void exec(Evalstate *state) override;
};

class INPUTsta : public Statement
{
    private:
        std::string name;
        virtual void createTree() override;
    public:
        INPUTsta(std::string name);
        virtual ~INPUTsta();
        virtual void exec(Evalstate *state) override;
};

#endif // STATEMENT_H
