#pragma once
#include <string>
#include <istream>

enum class TokenId{
INT_LIT,
STRING_LIT,
KW_VAR,
KW_FUNC,
KW_RETURN,
KW_IF,
KW_ELSE,
KW_FOR,
KW_BOOL,
KW_TRUE,
KW_FALSE,
KW_INT,
KW_REF,
KW_PRINT,
KW_PRINTLN,
IDENT,
OP_ADD,
OP_SUB,
OP_MUL,
OP_DIV,
OP_MOD,
OP_EQ,
OP_NEQ,
OP_LT,
OP_GT,
OP_LTE,
OP_GTE,
OP_AND,
OP_OR,
OP_NOT,
OP_ASSIGN,
OP_SHORT_DECL,
OP_ADDR,
OPEN_PAR,
CLOSE_PAREN,
OPEN_BRACE,
CLOSE_BRACE,
COMMA,
SEMICOLON,
END_OF_FILE,
UNK
};

struct Token{
    TokenId tokenId;
    std::string text;
    int line;
    int column;
};

class Lexer{
public:
    explicit Lexer(std::istream& input):input_(input),line_(1),col_(0){consume();}
    Token nextToken();
private:
    void consume(){
        if(ch_=='\n'){
            line_++;
            col_=0;
        }else{
            col_++;
        }
        ch_=input_.get();
    }
    std::istream& input_;
    char ch_;
    int line_=1;
    int col_=0;

};