#pragma once
#include "../lexer/Lexer.hpp"
#include <stdexcept>

class Parser{
public:
    explicit Parser(Lexer& lexer):lexer_(lexer){
        current_=lexer_.nextToken();
    }
    void parse();

private:
    void parseProgram();
    void parseTopDecl();
    void parseVarDecl();
    void parseType();
    void parseFuncDecl();
    void parseParamList();
    void parseParam();
    void parseBlock();
    void parseStmt();
    void parseShortDecl();
    void parseAssignStmt();
    void parseIfStmt();
    void parseForStmt();
    void parseReturnStmt();
    void parseCallStmt();
    void parsePrintStmt();
    void parsePrintArg();
    void parseArgList();
    void parseArg();
    void parseExpr();
    void parseOrExpr();
    void parseAndExpr();
    void parseNotExpr();
    void parseRelExpr();
    void parseRelOp();
    void parseAddExpr();
    void parseMulExpr();
    void parseUnaryExpr();
    void parsePrimary();
    void parseCallExpr();

    void consume(){
        current_=lexer_.nextToken();
    }

    void expect(TokenId expected);

    Lexer& lexer_;
    Token current_;
};