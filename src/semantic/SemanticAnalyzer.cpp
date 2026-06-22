#include "SemanticAnalyzer.hpp"

void SemanticAnalyzer::reportError(const std::string& error){

    errors_.push_back(error);
}

void SemanticAnalyzer::analyze(Program* program){
    symbols_.enterScopes();

    for(auto* decl:program->decls){
        analyzeDecl(decl);
    }
}

void SemanticAnalyzer::analyzeDecl(Decl* decl){

    if(auto* var=dynamic_cast<VarDecl*>(decl)){
        Symbol* symbol=new Symbol(var->name_,var->type_);
    
        if(!symbols_.declare(symbol)){
            reportError("Variable ya declarada : "+var->name_);
        }
    }else if(auto* func=dynamic_cast<FuncDecl*>(decl)){

        symbols_.enterScopes();

        for(const auto& param: func->params_){
            Symbol* symbol=new Symbol(param.name_,param.type_);

            if(!symbols_.declare(symbol)){
                reportError("Parametro redeclarado: "+param.name_);
            }
        }
        for(auto* stmt: func->block_->stms_){
            analyzeStmt(stmt);
        }
        symbols_.exitScopes();
    }
}

void SemanticAnalyzer::analyzeStmt(Stmt* stmt){
    if(auto* assign=dynamic_cast<AssignStmt*>(stmt)){
        Symbol*  symbol=symbols_.lookup(assign->name_);

        if(symbol==nullptr){
            reportError("Varianle no declarada: "+assign->name_);
        }
        analyzeExpr(assign->expr_);
    }else if(auto* var= dynamic_cast<VarDecl*>(stmt)){
        Symbol* symbol=new Symbol(var->name_,var->type_);

        if(!symbols_.declare(symbol)){
            reportError("Varianle ya declarada: "+assign->name_);
        }
    }
}

Type SemanticAnalyzer::analyzeExpr(Expr* expr){
    if(auto* inLit=dynamic_cast<IntLiteralExpr*>(expr)){
        return Type::Int;
    }

    if(auto* boolLit=dynamic_cast<BoolLiteralExpr*>(expr)){
        return Type::Bool;
    }

    if(auto* var=dynamic_cast<VariableExpr*>(expr)){
        Symbol* symbol= symbols_.lookup(var->name);

        if(symbol==nullptr){
            reportError("Varianle no declarada: "+var->name);
            return Type::Void;
        }
        return symbol->type_;
    }
    return Type::Void;
}
