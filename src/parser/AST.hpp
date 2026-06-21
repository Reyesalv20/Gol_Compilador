#pragma once
#include <string>
#include <vector>
enum class NodeKind{
    IntLiteral,
    BoolLiteral,
    Variable,
    BinaryExpr,
    Call,
    UnaryExpr,
    AddressOfExpr
};
enum class BinaryOp{
    Add,
    Sub,
    Mult,
    Div,
    Mod,
    Lt,
    Gt,
    Lte,
    Gte,
    Or,
    And,
    Not,
    Diff,
    Eq,
};

struct ASTNode{

    virtual ~ASTNode()=default;
    virtual NodeKind kind()=0;

};

struct Expr:ASTNode{

};

struct IntLiteralExpr: Expr{
    int value;
    explicit IntLiteralExpr(int v):value(v){}

    NodeKind kind() override{
        return NodeKind::IntLiteral;
    }

};

struct BoolLiteralExpr: Expr{
    bool value;
    explicit BoolLiteralExpr(bool v):value(v){}

    NodeKind kind() override{
        return NodeKind::BoolLiteral;
    }
};

struct VariableExpr: Expr{
    std::string name;
    explicit VariableExpr(const std::string& n):name(n){}

    NodeKind kind() override{
        return NodeKind::Variable;
    }
};

struct CallExpr: Expr{
    std::string functionName;
    std::vector<Expr*> arguments;

    explicit CallExpr(const std::string& name):functionName(name){}

    NodeKind kind() override {
        return NodeKind::Call;
    }
};

struct BinaryExpr:Expr{

    Expr* left;
    Expr* right;
    BinaryOp op;

    explicit BinaryExpr(BinaryOp operador,Expr* l,Expr* r):op(operador),left(l),right(r){}

    NodeKind kind() override{
        return NodeKind::BinaryExpr;
    }
};

struct UnaryExpr:Expr{
    char op;
    Expr* operand;
    explicit UnaryExpr(char operador,Expr* oper):op(operador),operand(oper){}

    NodeKind kind() override{
        return NodeKind::UnaryExpr;
    }

};

struct AddressOfExpr:Expr{
    Expr* operand;
    explicit AddressOfExpr(Expr* op):operand(op){}
    NodeKind kind() override{
        return NodeKind::AddressOfExpr;
    }
};