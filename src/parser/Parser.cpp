#include "Parser.hpp"
#include <iostream>

void Parser::expect(TokenId expected){
    if(current_.tokenId!=expected){
        throw std::runtime_error("Error en linea"+std::to_string(current_.line)+" :token inesperado ' "+ current_.text +" '");
    }
    consume();

}

void Parser::parse(){
    parseProgram();
    expect(TokenId::END_OF_FILE);
}

void Parser::parseProgram(){
    while(current_.tokenId!=TokenId::END_OF_FILE){
        parseTopDecl();
    }
}

void Parser::parseTopDecl(){
    if(current_.tokenId==TokenId::KW_FUNC){
        parseFuncDecl();
    }else if(current_.tokenId==TokenId::KW_VAR){
        parseVarDecl();
    }else{
       throw std::runtime_error("Error en linea "+std::to_string(current_.line)+ "se esperaba 'func' o 'var', llego ' "+current_.text+" '");
    }
}

void Parser::parseVarDecl(){
   
    expect(TokenId::KW_VAR);
    expect(TokenId::IDENT);
    parseType();

    if(current_.tokenId==TokenId::OP_ASSIGN){
        consume();
        parseExpr();
    }
   
    expect(TokenId::SEMICOLON);
    
}

void Parser::parseType(){
    if(current_.tokenId==TokenId::KW_INT||current_.tokenId==TokenId::KW_BOOL){
        consume();
    }else{
        throw std::runtime_error("Error en la linea "+std::to_string(current_.line)+" se esperaba 'int' o 'bool', llego ' "+current_.text+" '");
    }
}

void Parser::parseFuncDecl(){
    expect(TokenId::KW_FUNC);
    expect(TokenId::IDENT);
    expect(TokenId::OPEN_PAR);
    if(current_.tokenId==TokenId::KW_REF||current_.tokenId==TokenId::IDENT){
        parseParamList();
    }
    expect(TokenId::CLOSE_PAREN);
    if(current_.tokenId==TokenId::KW_INT||current_.tokenId==TokenId::KW_BOOL){
        parseType();
    }
    parseBlock();
}

void Parser::parseParamList(){
    parseParam();
    while(current_.tokenId==TokenId::COMMA){
        consume();
        parseParam();
    }
}
void Parser::parseParam(){
    if(current_.tokenId==TokenId::KW_REF){
        consume();
    }
    expect(TokenId::IDENT);
    parseType();

}


void Parser::parseBlock(){
   expect(TokenId::OPEN_BRACE);
        while(current_.tokenId==TokenId::KW_VAR||current_.tokenId==TokenId::IDENT||current_.tokenId==TokenId::KW_IF||current_.tokenId==TokenId::KW_FOR||current_.tokenId==TokenId::KW_RETURN||current_.tokenId==TokenId::KW_PRINT||current_.tokenId==TokenId::KW_PRINTLN||current_.tokenId==TokenId::OPEN_PAR){
            parseStmt();
        }
    expect(TokenId::CLOSE_BRACE);
}

void Parser::parseStmt(){
    if(current_.tokenId==TokenId::KW_VAR){
        parseVarDecl();
    }else if(current_.tokenId==TokenId::IDENT){
        expect(TokenId::IDENT);
        if(current_.tokenId==TokenId::OP_SHORT_DECL){
            parseShortDecl();
        }else if(current_.tokenId==TokenId::OP_ASSIGN){
            parseAssignStmt();
        }else if(current_.tokenId==TokenId::OPEN_PAR){
            parseCallStmt();
        }
    }else if(current_.tokenId==TokenId::KW_IF){
        parseIfStmt();
    }else if(current_.tokenId==TokenId::KW_FOR){
        parseForStmt();
    }else if(current_.tokenId==TokenId::KW_RETURN){
        parseReturnStmt();
    }else if(current_.tokenId==TokenId::KW_PRINT||current_.tokenId==TokenId::KW_PRINTLN||current_.tokenId==TokenId::OPEN_PAR){
        parsePrintStmt();
    }

}

void Parser::parseShortDecl(){
    expect(TokenId::OP_SHORT_DECL);
    parseExpr();
    expect(TokenId::SEMICOLON);
}

void Parser::parseAssignStmt(){
    expect(TokenId::OP_ASSIGN);
    parseExpr();
    expect(TokenId::SEMICOLON);
}

void Parser::parseIfStmt(){
    expect(TokenId::KW_IF);  
    parseExpr();
    parseBlock();
    while(current_.tokenId==TokenId::KW_ELSE){
        consume();            
        if(current_.tokenId==TokenId::KW_IF){
            consume();         
            parseExpr();
            parseBlock();
        } else {
            parseBlock();      
            break;            
        }
    }
}

void Parser::parseForStmt(){
    expect(TokenId::KW_FOR);
    parseExpr();
    parseBlock();
}

void Parser::parseReturnStmt(){
    expect(TokenId::KW_RETURN);
    if(current_.tokenId!=TokenId::SEMICOLON){
        parseExpr();
    }
    expect(TokenId::SEMICOLON);
}

void Parser::parseCallStmt(){
    expect(TokenId::OPEN_PAR);
    if(current_.tokenId!=TokenId::CLOSE_PAREN){
        parseArgList();
    }
    expect(TokenId::CLOSE_PAREN);
    expect(TokenId::SEMICOLON);
}

void Parser::parsePrintStmt(){
    if(current_.tokenId==TokenId::KW_PRINT||current_.tokenId==TokenId::KW_PRINTLN){
        consume();
    }else{
        throw std::runtime_error("Error en la linea "+std::to_string(current_.line)+" se esperaba 'print' o 'println', llego ' "+current_.text+" '");
    }
    expect(TokenId::OPEN_PAR);
    parsePrintArg();
    while(current_.tokenId==TokenId::COMMA){
        consume();
        parsePrintArg();
    }
    expect(TokenId::CLOSE_PAREN);
    expect(TokenId::SEMICOLON);
}

void Parser::parsePrintArg(){
    if(current_.tokenId==TokenId::STRING_LIT){
        consume();
    }else{
        parseExpr();
    }
}

void Parser::parseArgList(){
    parseArg();
    while(current_.tokenId==TokenId::COMMA){
        consume();
        parseArg();
    }
}

void Parser::parseArg(){
    if(current_.tokenId==TokenId::OP_ADDR){
        consume();
    }
    parseExpr();

}
//aqui
void Parser::parseExpr(){
    parseOrExpr();
}


void Parser::parseOrExpr(){
    parseAndExpr();
    while(current_.tokenId==TokenId::OP_OR){
        consume();
        parseAndExpr();
    }
}

void Parser::parseAndExpr(){
    parseNotExpr();
    while(current_.tokenId==TokenId::OP_AND){
        consume();
        parseNotExpr();
    }
}

void Parser::parseNotExpr(){
    if(current_.tokenId==TokenId::OP_NOT){
        consume();
        parseNotExpr();
    }else{
        parseRelExpr();
    }
}

void Parser::parseRelExpr(){
    parseAddExpr();
    if(current_.tokenId==TokenId::OP_EQ  || current_.tokenId==TokenId::OP_NEQ ||
       current_.tokenId==TokenId::OP_LT  || current_.tokenId==TokenId::OP_GT  ||
       current_.tokenId==TokenId::OP_LTE || current_.tokenId==TokenId::OP_GTE){
        consume();
        parseAddExpr();
    }
}

void Parser::parseRelOp(){
    if(current_.tokenId==TokenId::OP_EQ){
        consume();
    }else if(current_.tokenId==TokenId::OP_LT ){
        consume();
    }else if(current_.tokenId==TokenId::OP_LTE ){
        consume();
    }else if(current_.tokenId==TokenId::OP_NEQ){
        consume();
    }else if(current_.tokenId==TokenId::OP_GT){
        consume();
    }else if(current_.tokenId==TokenId::OP_GTE){
        consume(); 
    }
}

void Parser::parseAddExpr(){
    parseMulExpr();
    while(current_.tokenId==TokenId::OP_ADD||current_.tokenId==TokenId::OP_SUB){
        parseMulExpr();
    }
}

void Parser::parseMulExpr(){
    parseUnaryExpr();
    while(current_.tokenId==TokenId::OP_MOD||current_.tokenId==TokenId::OP_DIV||current_.tokenId==TokenId::OP_MUL){
        consume();
        parseUnaryExpr();
    }
}

void Parser::parseUnaryExpr(){
    if(current_.tokenId==TokenId::OP_SUB){
        consume();
        parseUnaryExpr();
    }else{
        parsePrimary();
    }
}

void Parser::parsePrimary(){
    if(current_.tokenId==TokenId::IDENT){
        consume();
        if(current_.tokenId==TokenId::OPEN_PAR){
            parseCallExpr();
        }
    }else if(current_.tokenId==TokenId::KW_TRUE){
        consume();
    }else if(current_.tokenId==TokenId::KW_FALSE){
        consume();
    }else if(current_.tokenId==TokenId::INT_LIT){
        consume();
    }else if(current_.tokenId==TokenId::OPEN_PAR){
        consume();
        parseExpr();
        expect(TokenId::CLOSE_PAREN);
    }

}

void Parser::parseCallExpr(){
    expect(TokenId::OPEN_PAR);
    if(current_.tokenId!=TokenId::CLOSE_PAREN){
        parseArgList();
    }
    expect(TokenId::CLOSE_PAREN);
}




