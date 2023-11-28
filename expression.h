#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include <sstream>
#include <evalstate.h>
#include <set>

namespace myExpr {

    static std::map<std::string, int> precedence = {
        {"(", 0}, {"+", 2}, {"-", 2}, {"*", 3}, {"/", 3}, {"**", 4}
    };

    static std::set<std::string> validOperators = {
        "+", "-", "*", "/", "**", "(", ")"
    };

    static std::set<std::string> banned =
        {"IF", "END", "REM", "THEN", "GOTO", "LET", "PRINT", "INPUT"};

    static const std::string pad = "    ";

    enum ExpressionType {CONSTANT, IDENTIFIER, COMPOUND};

    class Expression
    {
        public:
            Expression();
            std::vector<std::string> tree;
            virtual ~Expression() = 0;

            virtual int eval(Evalstate *state) = 0;
            virtual std::string toString() const = 0;
            virtual ExpressionType type() const = 0;
            virtual std::string getOperator() const;
            virtual Expression *getLeft() const;
            virtual Expression *getRight() const;
    };

    class ConstantExp : public Expression
    {
        public:
           ConstantExp(int value);
           virtual ~ConstantExp();
           virtual int eval(Evalstate *state) override;
           virtual std::string toString() const override;
           virtual ExpressionType type() const override;
        private:
           int value;
    };

    class IdentifierExp : public Expression
    {
        public:
           IdentifierExp(std::string n);
           virtual ~IdentifierExp();
           virtual int eval(Evalstate *state) override;
           virtual std::string toString() const override;
           virtual ExpressionType type() const override;
        private:
           std::string name;
    };

    class CompoundExp : public Expression {
        public:
           CompoundExp(std::string op, Expression *left, Expression *right);
           virtual ~CompoundExp();

           virtual int eval(Evalstate *state) override;
           virtual std::string toString() const override;
           virtual ExpressionType type() const override;
           std::string getOperator() const override;

           virtual Expression *getLeft() const override;
           virtual Expression *getRight() const override;

        private:
           std::string op;
           Expression *left, *right;
    };

}

#endif // EXPRESSION_H
