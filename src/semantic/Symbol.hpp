#include <string>
#include <vector>
#include "../parser/AST.hpp"

struct Symbol{
    virtual ~Symbol() = default;
    std::string name_;
    Type type_;

    explicit Symbol(const std::string& name,Type type):name_(name),type_(type){}

};

struct FuncSymbol : Symbol{

    std::vector<Type> param_type_;

    FuncSymbol(const std::string& name,Type returnType,const std::vector<Type>& param_type):
    Symbol(name,returnType),param_type_(param_type){}
};