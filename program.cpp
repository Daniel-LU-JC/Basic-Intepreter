#include "program.h"
#include "myexception.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace QBasic;

Program::Program(std::string fileName)
{
    int n;
    std::ifstream f(fileName);
    std::string s;
    std::stringstream ss;

    while (getline(f, s)) {
        ss.clear();
        ss << s;  // from std::string to stringstream
        ss >> n;  // get the line number divided by space using stringstream
        getline(ss, s);  // from stringstream to std::string
        appendCMD(n, s);
    }

    initTools();

    rip = statements.end();
}

Program::Program()
{
    initTools();
    rip = statements.end();
}

void Program::initTools()
{
    tokenizer = new Tokenizer;
    parser = new Parser;
    evalstate = new Evalstate;
}

Program::~Program()
{
    delete tokenizer;
    delete parser;
    delete evalstate;
}

void Program::getTokens()  // using the tool 'tokenizer'
{
    commands.clear();
    error.clear();
    for (auto & cmd : rawCommands) {
        try {
            commands[cmd.first] = tokenizer->string2tokens(cmd.second);
        } catch (IdentifierError &e) {
            error[cmd.first] = e.what();
        }
    }
}

Statement * Program::getOneStatements(Token token)
{
    Expression * exp = nullptr;
    Expression * exp1 = nullptr;
    Statement * sta = nullptr;

    switch (hash(token.at(0))) {

        case LET: {
            try {
                token.at(3);
            } catch (std::out_of_range &e) {
                throw StatementError("LET");
            }

            if (myExpr::banned.count(token[1]))
                throw IdentifierError(token[1]);

            Token expr(token.begin() + 3, token.end());

            try {
                exp = parser->token2Exp(expr);  // from token string to expression
                sta = new LETsta(token[1], exp);  // from vector to LET statement
            } catch (IdentifierExp &e) {
                throw e;
            } catch(Myexception &e){
                throw ExpressionError(tokenizer->tokens2string(expr));
            }

            break;
        }

        case PRINT: {
            try {
                token.at(1);
            } catch (std::out_of_range &e) {
                throw StatementError("PRINT");
            }

            Token expr(token.begin() + 1, token.end());

            try {
                exp = parser->token2Exp(expr);
                sta = new PRINTsta(exp);
            } catch (IdentifierExp &e) {
                throw e;
            } catch (Myexception &e){
                throw ExpressionError(tokenizer->tokens2string(expr));
            }

            break;
        }

        case GOTO: {
            try {
                token.at(1);
            } catch (std::out_of_range &e) {
                throw StatementError("GOTO");
            }

            if(token.size() > 2)
                throw StatementError("GOTO");

            int n = stoi(token[1]);

            if (rawCommands.count(n)) {
                sta = new GOTOsta(n);
            } else {
                throw LineError(token[1]);
            }

            break;
        }

        case IF: {
            try {
                token.at(5);
            } catch (std::out_of_range &e) {
                throw StatementError("IF THEN");
            }

            int n = stoi(*(token.end() - 1));
            if(!rawCommands.count(n)) {
                throw LineError(*(token.end()-1));
            }

            // n IF exp1 op exp2 THEN n1
            auto op = token.end();  // find the operator location
            for (const auto &validOp: validCompareOperators) {
                if (op != token.end()) break;
                op = find(token.begin(), token.end(), validOp);
            }

            if (op == token.end()) {
                throw StatementError("IF");
            }

            Token token1(token.begin()+1,op);
            Token token2(op+1,token.end()-2);

            try {
                exp = parser->token2Exp(token1);
            } catch (IdentifierExp &e) {
                throw e;
            } catch (Myexception &e){
                throw ExpressionError(tokenizer->tokens2string(token1));
            }

            try {
                exp1 = parser->token2Exp(token2);
                sta = new IFsta(*op, exp, exp1, n);
            } catch (IdentifierExp &e) {
                throw e;
            } catch (Myexception &e){
                throw ExpressionError(tokenizer->tokens2string(token2));
            }

            break;
        }

        case INPUT:{
            try {
                token.at(1);
            } catch (std::out_of_range &e) {
                throw StatementError("INPUT");
            }

            if(token.size()>2){
                throw StatementError("INPUT");
            }

            if(myExpr::banned.count(token[1])){
                throw IdentifierError(token[1]);
            }

            sta = new INPUTsta(token[1]);

            break;
        }

        case REM: {
            try {
                token.at(1);
            } catch (std::out_of_range &e) {
                throw StatementError("REM");
            }

            // recover the vector of strings into one single string
            sta = new REMsta(tokenizer->tokens2string(Token(token.begin()+1,token.end())));

            break;
        }

        case END:

            if (token.size()>1) {
                throw StatementError("END");
            }

            sta = new ENDsta();

            break;

        default:
            throw StatementError(token[0] + " is not a valid statement");
            break;
    }

    return sta;
}

// parse all the lines into statements
void Program::getStatements()
{
    statements.clear();
    for(auto &cmd : commands) {
        int num = cmd.first;
        Token line(cmd.second);
        try {
            Statement *sta = getOneStatements(line);
            statements[num] = sta;
        } catch (Myexception &e) {
            error[num] = e.what();
        }
    }
}

void Program::getTree()
{
    tree.clear();
    for (auto &sta : statements) {
        tree[sta.first] = sta.second->getTree();
    }  // generate the tree structure for each statement

    for (auto &err : error) {
        tree[err.first] = std::vector<std::string>{err.second};
    }
}

// delete a certain command based on the line number
void Program::removeCmd(int n)
{
    for (auto iter = rawCommands.begin(); iter!=rawCommands.end(); iter++) {
        if (iter->first == n) {
            rawCommands.erase(iter);
            break;
        }
    }
}

// string to integer
int Program::stoi(std::string s)
{
    std::stringstream ss(s);
    int res;
    ss >> res;
    return res;
}

void Program::exec()
{
    getTokens();
    getStatements();
    getTree();  // parse the semantics of each command and make preparations
    evalstate->reset();  // clear all the variables and outputs
    rip = statements.begin();  // start from the beginning
    _exec();
}

void Program::_exec()
{
    Statement *sta = nullptr;
    int next;
    while (rip != statements.end()) {
        sta = rip -> second;  // get the statement from rip
        try {
            sta->exec(evalstate);  // execute
        } catch (Myexception &e) {
            evalstate->print(e.what());
        }

        next = evalstate->getNextLineNumber();

        switch (next) {
            case Evalstate::end:
                rip = statements.end();
                break;
            case Evalstate::next:
                rip++;
                break;
            default:
                if (error.count(next)) {
                    rip++;
                } else {
                    rip = statements.find(next);  // IF or GOTO
                }
        }

        // suspend the program and wait for the inputs from user
        if (evalstate->isSuspended()) {
            break;
        }
    }
}

void Program::continueExec(std::string n)
{
    try {
        int value = stoi(n);
        if (value == 0 && n[0] != 0) {
            evalstate->finishInput(value,false);
            throw InputError(n);
        } else {
            evalstate->finishInput(value);
        }
    } catch (InputError &e) {
        evalstate->print(e.what());
    }

    if (rip != statements.end())
        _exec();
}

int Program::hash(std::string s)
{
    int sum = 0;
    for (char c: s) {
        sum <<= 8;
        sum += c;
    }
    return sum % e;
}

void Program::appendCMD(int lineNumber, std::string cmd)
{
    // remove the command if cmd is empty
    if (rawCommands.count(lineNumber) && cmd.empty()) {
        removeCmd(lineNumber);
        return;
    }

    if (!cmd.empty())
        rawCommands[lineNumber] = cmd;  // raw commands before tokenization
}

std::vector<std::string> Program::getOutput() {
    return evalstate->getOutput();
}

bool Program::isSuspended() const {
    return evalstate->isSuspended();
}

void Program::execOne(std::string cmd) {
    try {
        Token token = tokenizer->string2tokens(cmd);
        Statement * sta = getOneStatements(token);
        sta->exec(evalstate);
    } catch (Myexception &e) {
        evalstate->print(e.what());
    }
}
