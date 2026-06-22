#pragma once
#include <vector>
#include <string>
#include "SymbolTable.hpp"
#include "../parser/AST.hpp"

class SemanticAnalyzer{

    public:

    void analyze(Program* program);
    const std::vector<std::string>& errors() const{
        return errors_;
    }
    private:

    SymbolTable symbols_;
    std::vector<std::string> errors_;

    void analyzeDecl(Decl* decl);
    void analyzeStmt(Stmt* stmt);
    Type analyzeExpr(Expr* expr);
    void reportError(const std::string& error);
}; 
