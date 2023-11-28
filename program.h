#ifndef PROGRAM_H
#define PROGRAM_H

#include<iostream>
#include<vector>
#include<map>

#include<tokenizer.h>
#include<statement.h>
#include<expression.h>
#include<parser.h>

namespace QBasic {

    // Tree: lines in the syntax tree are stored in the vector
    using Tree = std::vector<std::string>;
    // Token: tokens in the command line are stored in the vector
    using Token = std::vector<std::string>;

    static std::set<std::string> validCommand
        {"LET", "IF", "REM", "END", "GOTO", "PRINT", "INPUT"};

    static std::set<std::string> validCompareOperators
        {"=", "<", ">"};

    class Program
    {
        enum {e = 98999};
        enum {LET = 48534};
        enum {IF = 18758};
        enum {REM = 45747};
        enum {END = 87065};
        enum {GOTO = 77347};
        enum {PRINT = 93836};
        enum {INPUT = 71843};  // Hash value of the strings for 'switch' structure

        public:

            Program();  // construct an empty basic program
            ~Program();
            Program(std::string fileName);  // contruct a program from an existing file

            static int stoi(std::string s);
            std::map <int, std::string> rawCommands;
            std::map <int, Token> commands;
            std::map <int, Statement *> statements;
            std::map <int, std::string> error;
            std::map <int, Tree> tree;

            void getTokens();
            void getStatements();
            void getTree();

            Statement * getOneStatements(Token token);
            void exec();
            void continueExec(std::string n);
            void appendCMD(int lineNumber, std::string cmd);

            std::vector<std::string> getOutput();
            bool isSuspended()const;
            void execOne(std::string cmd);

        private:

            int hash(std::string s);
            void removeCmd(int n);
            void initTools();
            void _exec();

            std::map <int, Statement *>::iterator rip;
            Tokenizer * tokenizer;
            Parser * parser;
            Evalstate * evalstate;
    };
}

#endif // PROGRAM_H
