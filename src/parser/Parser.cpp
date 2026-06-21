#include "Parser.hpp"
#include <iostream>

void Parser::expect(TokenId expected){
    if(current_.tokenId!=expected){
        throw std::runtime_error("Error en linea "+std::to_string(current_.line)+" :token inesperado ' "+ current_.text +" '");
    }
    consume();

}

Program* Parser::parse(){
    Program* program=parseProgram();
    expect(TokenId::END_OF_FILE);
    return program;
}

Program* Parser::parseProgram(){
    Program* program=new Program();
    while(current_.tokenId!=TokenId::END_OF_FILE){
        program->decls.push_back(parseTopDecl());
    }
    return program;
}

Decl* Parser::parseTopDecl(){
    if(current_.tokenId==TokenId::KW_FUNC){
        return parseFuncDecl();
    }else if(current_.tokenId==TokenId::KW_VAR){
        return parseVarDecl();
    }else{
       throw std::runtime_error("Error en linea "+std::to_string(current_.line)+ "se esperaba 'func' o 'var', llego ' "+current_.text+" '");
    }
}

VarDecl* Parser::parseVarDecl(){
   
    expect(TokenId::KW_VAR);
    
    std::string name=current_.text;
    expect(TokenId::IDENT);
    Type type=parseType();
    Expr* expr=nullptr;
    if(current_.tokenId==TokenId::OP_ASSIGN){
        consume();
        expr= parseExpr();
    }
   
    expect(TokenId::SEMICOLON);
    return new VarDecl(name,type,expr);
}

Type Parser::parseType(){
    if(current_.tokenId==TokenId::KW_INT){
        consume();
        return Type::Int;
    }else if(current_.tokenId==TokenId::KW_BOOL){
        consume();
        return Type::Bool;
    }else{
        throw std::runtime_error("Error en la linea "+std::to_string(current_.line)+" se esperaba 'int' o 'bool', llego ' "+current_.text+" '");
    }
}

FuncDecl* Parser::parseFuncDecl(){
    expect(TokenId::KW_FUNC);
    std::string name=current_.text;
    expect(TokenId::IDENT);
    expect(TokenId::OPEN_PAR);
    std::vector<Param> params;
    if(current_.tokenId==TokenId::KW_REF||current_.tokenId==TokenId::IDENT){
        params=parseParamList();
    }
    expect(TokenId::CLOSE_PAREN);
    Type type=Type::Void;
    if(current_.tokenId==TokenId::KW_INT||current_.tokenId==TokenId::KW_BOOL){
        type=parseType();
    }
    BlockStmt* block=parseBlock();

    return new FuncDecl(name,params,type,block);
}

std::vector<Param> Parser::parseParamList(){
    std::vector<Param> params;
    params.push_back(parseParam());
    while(current_.tokenId==TokenId::COMMA){
        consume();
        params.push_back(parseParam());
    }
    return params;
}
Param Parser::parseParam(){
    bool isRef = false;
    if(current_.tokenId==TokenId::KW_REF){
        consume();
        isRef=true;
    }
    std::string name=current_.text;
    expect(TokenId::IDENT);
    Type type=parseType();
    return Param(isRef,name,type);
}


BlockStmt* Parser::parseBlock(){
   std::vector<Stmt*> stmts;
   expect(TokenId::OPEN_BRACE);
        while(current_.tokenId==TokenId::KW_VAR||current_.tokenId==TokenId::IDENT||current_.tokenId==TokenId::KW_IF||current_.tokenId==TokenId::KW_FOR||current_.tokenId==TokenId::KW_RETURN||current_.tokenId==TokenId::KW_PRINT||current_.tokenId==TokenId::KW_PRINTLN||current_.tokenId==TokenId::OPEN_PAR){
           stmts.push_back(parseStmt());
        }
    expect(TokenId::CLOSE_BRACE);
    return new BlockStmt(stmts);
}

Stmt* Parser::parseStmt(){
    if(current_.tokenId==TokenId::KW_VAR){
        return parseVarDecl();
    }else if(current_.tokenId==TokenId::IDENT){
        std::string name=current_.text;
        consume();
        return parseIdentStmt(name);
    }else if(current_.tokenId==TokenId::KW_IF){
        return parseIfStmt();
    }else if(current_.tokenId==TokenId::KW_FOR){
        return parseForStmt();
    }else if(current_.tokenId==TokenId::KW_RETURN){
        return parseReturnStmt();
    }else if(current_.tokenId==TokenId::KW_PRINT||current_.tokenId==TokenId::KW_PRINTLN||current_.tokenId==TokenId::OPEN_PAR){
        return parsePrintStmt();
    }else{
         throw std::runtime_error("Error en la linea "+std::to_string(current_.line)+" llego un token inesperado ' "+current_.text+" '");
    }

}

Stmt* Parser::parseIdentStmt(const std::string name){
    if(current_.tokenId==TokenId::OP_SHORT_DECL){
            return parseShortDecl(name);
    }else if(current_.tokenId==TokenId::OP_ASSIGN){
            return parseAssignStmt(name);
    }else if(current_.tokenId==TokenId::OPEN_PAR){
            return parseCallStmt(name);
    }else{
         throw std::runtime_error("Error en la linea "+std::to_string(current_.line)+" llego tocken inesperado ' "+current_.text+" '");
    }
}
ShortDecl* Parser::parseShortDecl(const std::string name){
    expect(TokenId::OP_SHORT_DECL);
    Expr* expr=parseExpr();
    expect(TokenId::SEMICOLON);
    return new ShortDecl(name,expr);
}

AssignStmt* Parser::parseAssignStmt(const std::string name){
    expect(TokenId::OP_ASSIGN);
    Expr* expr=parseExpr();
    expect(TokenId::SEMICOLON);
    return new AssignStmt(name,expr);
}

IfStmt* Parser::parseIfStmt(){
    expect(TokenId::KW_IF);  
    Expr* cond=parseExpr();
    BlockStmt* thenBlock=parseBlock();
    std::vector<ElseIf> elseIfs;

    BlockStmt* elseBlock=nullptr;
    while(current_.tokenId==TokenId::KW_ELSE){
        consume();
        if(current_.tokenId==TokenId::KW_IF){
            consume();
            ElseIf branch;
            branch.condition_ = parseExpr();
            branch.block_ = parseBlock();
            elseIfs.push_back(branch);
        }else{
            elseBlock=parseBlock();
            break;
        }            
        
    }
    return new IfStmt(cond,thenBlock,elseIfs,elseBlock);
}
/*void Parser::parseIfStmtPrime(){
    if(current_.tokenId==TokenId::KW_IF){
        parseIfStmt();
    }else if(current_.tokenId==TokenId::OPEN_BRACE){
        parseBlock();
    }

} */


ForStmt* Parser::parseForStmt(){
    expect(TokenId::KW_FOR);
    Expr* expr=parseExpr();
    BlockStmt* block= parseBlock();
    return new ForStmt(expr,block);
}

ReturnStmt* Parser::parseReturnStmt(){
    expect(TokenId::KW_RETURN);
    Expr* expr=nullptr;
    if(current_.tokenId!=TokenId::SEMICOLON){
        expr=parseExpr();
    }
    expect(TokenId::SEMICOLON);
    return new ReturnStmt(expr);
}

CallStmt* Parser::parseCallStmt(const std::string name){
    expect(TokenId::OPEN_PAR);
    std::vector<Expr*> args;
    if(current_.tokenId!=TokenId::CLOSE_PAREN){
        args=parseArgList();
    }
    expect(TokenId::CLOSE_PAREN);
    expect(TokenId::SEMICOLON);
    CallExpr* call=new CallExpr(name);
    call->arguments=args;

    return new CallStmt(call);
}

PrintStmt* Parser::parsePrintStmt(){
    bool isPrintLn=false;
    if(current_.tokenId==TokenId::KW_PRINT){
        consume();
        isPrintLn=false;
    }else if(current_.tokenId==TokenId::KW_PRINTLN){
         consume();
         isPrintLn=true;
    }else{
        throw std::runtime_error("Error en la linea "+std::to_string(current_.line)+" se esperaba 'print' o 'println', llego ' "+current_.text+" '");
    }
    expect(TokenId::OPEN_PAR);
    std::vector<PrintArg> args;
    args.push_back(parsePrintArg());
    while(current_.tokenId==TokenId::COMMA){
        consume();
        args.push_back(parsePrintArg());
    }
    expect(TokenId::CLOSE_PAREN);
    expect(TokenId::SEMICOLON);
    return new PrintStmt(isPrintLn,args);
}

PrintArg Parser::parsePrintArg(){
    PrintArg arg;
    arg.isString_=false;
    arg.string_="";
    arg.expr_=nullptr;
    if(current_.tokenId==TokenId::STRING_LIT){
        arg.string_=current_.text;
        arg.isString_=true;
        consume();
    }else{
       arg.expr_=parseExpr();
    }
    return arg;
}

std::vector<Expr*> Parser::parseArgList(){
    std::vector<Expr*> args;
    args.push_back( parseArg());
    while(current_.tokenId==TokenId::COMMA){
        consume();
        args.push_back(parseArg());
        
    }
    return args;
}

Expr* Parser::parseArg(){
    if(current_.tokenId==TokenId::OP_ADDR){
        consume();
        Expr* operand=parseExpr();
        return new AddressOfExpr(operand);
    }
    return parseExpr();

}

Expr* Parser::parseExpr(){
    return parseOrExpr();
}


Expr* Parser::parseOrExpr(){
    Expr* left=parseAndExpr();
    while(current_.tokenId==TokenId::OP_OR){
        consume();
        Expr* right=parseAndExpr();
        left=new BinaryExpr(BinaryOp::Or,left,right);
    }
    return left;
}

Expr* Parser::parseAndExpr(){
    Expr* left=parseNotExpr();
    while(current_.tokenId==TokenId::OP_AND){
        consume();
        Expr* right=parseNotExpr();
        left=new BinaryExpr(BinaryOp::And,left,right);
    }
    return left;
}

Expr* Parser::parseNotExpr(){
    if(current_.tokenId==TokenId::OP_NOT){
        consume();
        Expr* operand=parseNotExpr();
        return new UnaryExpr('!',operand);
    }
      return parseRelExpr();
    
}

Expr* Parser::parseRelExpr(){
    Expr* left=parseAddExpr();
    if(current_.tokenId==TokenId::OP_EQ  || current_.tokenId==TokenId::OP_NEQ ||
       current_.tokenId==TokenId::OP_LT  || current_.tokenId==TokenId::OP_GT  ||
       current_.tokenId==TokenId::OP_LTE || current_.tokenId==TokenId::OP_GTE){
        BinaryOp op=parseRelOp();
        Expr* right=parseAddExpr();
        left=new BinaryExpr(op,left,right);
    }
    return left;
}

BinaryOp Parser::parseRelOp(){
    if(current_.tokenId==TokenId::OP_EQ){
        consume();
        return BinaryOp::Eq;
    }else if(current_.tokenId==TokenId::OP_LT ){
        consume();
        return BinaryOp::Lt;
    }else if(current_.tokenId==TokenId::OP_LTE ){
        consume();
        return BinaryOp::Lte;
    }else if(current_.tokenId==TokenId::OP_NEQ){
        consume();
        return BinaryOp::Diff;
    }else if(current_.tokenId==TokenId::OP_GT){
        consume();
        return BinaryOp::Gt;
    }else if(current_.tokenId==TokenId::OP_GTE){
        consume(); 
        return BinaryOp::Gte;
    }else{
        throw std::runtime_error("Operador relacional invalido");
    }
}

Expr* Parser::parseAddExpr(){
    BinaryOp op;
    Expr* left=parseMulExpr();
    while(current_.tokenId==TokenId::OP_ADD||current_.tokenId==TokenId::OP_SUB){
        if(current_.tokenId==TokenId::OP_ADD){
            op=BinaryOp::Add;
        }else if(current_.tokenId==TokenId::OP_SUB){
            op=BinaryOp::Sub;
        }
        consume();
        Expr* right=parseMulExpr();
        left=new BinaryExpr(op,left,right);
    }
    return left;
}

Expr* Parser::parseMulExpr(){
    BinaryOp op;
    Expr* left=parseUnaryExpr();
    while(current_.tokenId==TokenId::OP_MOD||current_.tokenId==TokenId::OP_DIV||current_.tokenId==TokenId::OP_MUL){
        if(current_.tokenId==TokenId::OP_MOD){
            op=BinaryOp::Mod;
        }else if(current_.tokenId==TokenId::OP_DIV){
             op=BinaryOp::Div;
        }else if(current_.tokenId==TokenId::OP_MUL){
             op=BinaryOp::Mult;
        }

        consume();
        Expr* right= parseUnaryExpr();
        left=new BinaryExpr(op,left,right);
    }
    return left;
}

Expr* Parser::parseUnaryExpr(){
    if(current_.tokenId==TokenId::OP_SUB){
        consume();
        Expr* operand= parseUnaryExpr();
        return new UnaryExpr('-',operand);
    }
    
    return parsePrimary();
    
}

Expr* Parser::parsePrimary(){
    if(current_.tokenId==TokenId::IDENT){
        std::string name=current_.text;
        consume();
        return parsePrimaryPrime(name);
    }else if(current_.tokenId==TokenId::KW_TRUE){
        bool value=true;
        consume();
        return new BoolLiteralExpr(value);
    }else if(current_.tokenId==TokenId::KW_FALSE){
        bool value=false;
        consume();
        return new BoolLiteralExpr(value);
    }else if(current_.tokenId==TokenId::INT_LIT){
        
        int value=std::stoi(current_.text);
        consume();
        return new IntLiteralExpr(value);

    }else if(current_.tokenId==TokenId::OPEN_PAR){
        consume();
        Expr* expr=parseExpr();
        expect(TokenId::CLOSE_PAREN);
        return expr;
    }else {
        throw std::runtime_error(
            "Error linea " + std::to_string(current_.line) +
            ": expresion invalida '" + current_.text + "'"
        );
    }

}
Expr* Parser::parsePrimaryPrime(const std::string& name){
    if(current_.tokenId==TokenId::OPEN_PAR){
        return parseCallExpr(name);
    }
    return new VariableExpr(name);
}

Expr* Parser::parseCallExpr(const std::string& name){
    expect(TokenId::OPEN_PAR);
    std::vector<Expr*> args;
    if(current_.tokenId!=TokenId::CLOSE_PAREN){
       args=parseArgList();
    }
    expect(TokenId::CLOSE_PAREN);
    CallExpr* call=new CallExpr(name);
    call->arguments=args;
    return call;

}




