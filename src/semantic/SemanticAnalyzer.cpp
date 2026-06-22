#include "SemanticAnalyzer.hpp"

void SemanticAnalyzer::reportError(const std::string& error){

    errors_.push_back(error);
}

void SemanticAnalyzer::analyze(Program* program){
    symbols_.enterScopes();
    bool hasMain = false;
    for(auto* decl:program->decls){
        if(auto* func=dynamic_cast<FuncDecl*>(decl)){
            if(func->name_=="main"){
                hasMain=true;
            }
        }
        analyzeDecl(decl);
    }
    if(!hasMain){
    reportError("Funcion main no encontrada");
    }
}

void SemanticAnalyzer::analyzeDecl(Decl* decl){

    if(auto* var=dynamic_cast<VarDecl*>(decl)){
        Symbol* symbol=new Symbol(var->name_,var->type_);
    
        if(!symbols_.declare(symbol)){
            reportError("Variable ya declarada : "+var->name_);
        }
        if(var->init_!=nullptr){
            Type initType=analyzeExpr(var->init_);

            if(initType!=var->type_){
                reportError("Type mismatch en inicializacion de : "+var->name_);
            }
        }
    }else if(auto* func=dynamic_cast<FuncDecl*>(decl)){
        currentReturnType_=func->type_;
        std::vector<Type> paramTypes;

        for(const auto& param:func->params_){
            paramTypes.push_back(param.type_);
        }

        FuncSymbol* funcSymbol= new FuncSymbol(func->name_,func->type_,paramTypes);
        if(!symbols_.declare(funcSymbol)){
            reportError("Funcion redeclarada:  "+func->name_);
        }
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
            reportError("Variable no declarada: "+assign->name_);
        }else{
            Type exprType= analyzeExpr(assign->expr_);

            if(symbol->type_!=exprType){
                reportError("Type mismatch en assignacion a : "+assign->name_);
            }
        }
       
    }else if(auto* var= dynamic_cast<VarDecl*>(stmt)){
        Symbol* symbol=new Symbol(var->name_,var->type_);

        if(!symbols_.declare(symbol)){
            reportError("Varianle ya declarada: "+var->name_);
        }
        if(var->init_!=nullptr){
            Type initType=analyzeExpr(var->init_);

            if(initType!=var->type_){
                reportError("Type mismatch en inicializacion de : "+var->name_);
            }
        }
    }else if(auto* ret=dynamic_cast<ReturnStmt*>(stmt)){
            Type returnType=Type::Void;

            if(ret->expr_!=nullptr){
                returnType=analyzeExpr(ret->expr_);
            }
            if(returnType!=currentReturnType_){
                reportError("Return Type mismatch ");
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
    if(auto* bin=dynamic_cast<BinaryExpr*>(expr)){
        Type leftType=analyzeExpr(bin->left);

        Type rightType=analyzeExpr(bin->right);

        if(leftType!=rightType){
            reportError("Type mismatch en expresion binaria ");    
            return Type::Void;
        }
        return leftType;
    }

    if(auto* call=dynamic_cast<CallExpr*>(expr)){
        
        Symbol* symbol = symbols_.lookup(call->functionName);

        if(symbol==nullptr){
            reportError("Funcion no declarada "+call->functionName);
            return Type::Void;
        }
        auto* func = dynamic_cast<FuncSymbol*>(symbol);

        if(func==nullptr){
            reportError(call->functionName+" no es una funcion");
            return Type::Void;
        }
        
        if(call->arguments.size()!=func->param_type_.size()){
            reportError("Cantidad incorrecta de argumentos para: "+call->functionName);
        }
        return func->type_;
        
    }
    return Type::Void;
}
