#pragma once
#include "../lexer/Lexer.hpp"
#include "AST.hpp"
#include <stdexcept>

class Parser{
public:
    explicit Parser(Lexer& lexer):lexer_(lexer){
        current_=lexer_.nextToken();
    }
    Program* parse();

private:
    Program* parseProgram();
    Decl* parseTopDecl();
    VarDecl* parseVarDecl();
    Type parseType();
    FuncDecl* parseFuncDecl();
    std::vector<Param> parseParamList();
    Param parseParam();
    BlockStmt* parseBlock();
    Stmt* parseStmt();
    Stmt* parseIdentStmt(const std::string& name);
    ShortDecl* parseShortDecl(const std::string& name);
    AssignStmt* parseAssignStmt(const std::string& name);
    IfStmt* parseIfStmt();
    //void parseIfStmtPrime();
    ForStmt* parseForStmt();
    ReturnStmt* parseReturnStmt();
    CallStmt* parseCallStmt(const std::string& name);
    PrintStmt* parsePrintStmt();
    PrintArg parsePrintArg();
    std::vector<Expr*> parseArgList();
    Expr* parseArg();
    Expr* parseExpr();
    Expr* parseOrExpr();
    Expr* parseAndExpr();
    Expr* parseNotExpr();
    Expr* parseRelExpr();
    BinaryOp parseRelOp();
    Expr* parseAddExpr();
    Expr* parseMulExpr();
    Expr* parseUnaryExpr();
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