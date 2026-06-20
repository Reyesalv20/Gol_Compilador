#pragma once
#include "../lexer/Lexer.hpp"
#include "AST.hpp"
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
    void parseIdentStmt();
    void parseShortDecl();
    void parseAssignStmt();
    void parseIfStmt();
    void parseIfStmtPrime();
    void parseForStmt();
    void parseReturnStmt();
    void parseCallStmt();
    void parsePrintStmt();
    void parsePrintArg();
    void parseArgList();
    void parseArg();
    Expr* parseExpr();
    void parseOrExpr();
    void parseAndExpr();
    void parseNotExpr();
    void parseRelExpr();
    void parseRelOp();
    void parseAddExpr();
    void parseMulExpr();
    void parseUnaryExpr();
    Expr* parsePrimary();
    Expr* parseCallExpr(const std::string& name);
    Expr* parsePrimaryPrime(const std::string& name);

    void consume(){
        current_=lexer_.nextToken();
    }

    void expect(TokenId expected);

    Lexer& lexer_;
    Token current_;
};