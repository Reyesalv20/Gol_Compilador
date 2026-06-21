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
    AddressOfExpr,
    
    Program,
    
    VarDecl,
    FuncDecl,

    BlockSmt,
    ReturnStmt,
    IfStmt,
    ForStmt,
    ShortDecl,
    AssignStmt,
    PrintStmt,
    CallStmt
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
    Diff,
    Eq,
};

enum class Type{
    Int,
    Bool,
    Void
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

struct Stmt:ASTNode{

};

struct Decl:ASTNode{
    
};

struct Program: ASTNode{
    std::vector<Decl*> decls;
    
    NodeKind kind() override{
        return NodeKind::Program;
    }
};

struct VarDecl: Decl,Stmt{

    std::string name_;
    Type type_;
    Expr* init_;

    explicit VarDecl(const std::string& name,Type type,Expr* init):name_(name),type_(type),init_(init){}
    NodeKind kind() override{
        return NodeKind::VarDecl;
    }
};

struct Param{
  bool isRef_;
  std::string name_;
  Type type_;  

  explicit Param(bool isRef,std::string name,Type type):isRef_(isRef),name_(name),type_(type){}
};

struct BlockStmt : Stmt{
    std::vector<Stmt*> stms_;

    explicit BlockStmt(std::vector<Stmt*> stms):stms_(stms){}
    NodeKind kind() override{
        return NodeKind::BlockSmt;
    }
};

struct FuncDecl: Decl{

    std::string name_;
    std::vector<Param> params_;
    Type type_;
    BlockStmt* block_;
    
    explicit FuncDecl(const std::string& name,std::vector<Param> params,Type type,BlockStmt* block):name_(name),params_(params),type_(type),block_(block){}
    NodeKind kind() override{
        return NodeKind::FuncDecl;
    }
};

struct ShortDecl: Stmt{
    std::string name_;
    Expr* expr_;

    explicit ShortDecl(const std::string& name,Expr* expr):name_(name),expr_(expr){}

    NodeKind kind() override{
        return NodeKind::ShortDecl;
    }

};

struct AssignStmt: Stmt{
    std::string name_;
    Expr* expr_;
    explicit AssignStmt(const std::string& name,Expr* expr):expr_(expr),name_(name){}

    NodeKind kind() override{
        return NodeKind::AssignStmt;
    }

};

struct ReturnStmt: Stmt{
    Expr* expr_;
    explicit ReturnStmt(Expr* expr):expr_(expr){}

    NodeKind kind() override{
        return NodeKind::ReturnStmt;
    }
};

struct ForStmt: Stmt{
    Expr* expr_;
    BlockStmt* block_;
    explicit ForStmt(Expr* expr,BlockStmt* block):expr_(expr),block_(block){}

    NodeKind kind() override{
        return NodeKind::ForStmt;
    }
};
struct ElseIf{
    Expr* condition_;
    BlockStmt* block_;
};
struct IfStmt: Stmt{
    Expr* expr_;
    BlockStmt* thenblock_;
    std::vector<ElseIf> elseIfs_;
    BlockStmt* elseBlock_;
    explicit IfStmt(Expr* expr,BlockStmt* block,const std::vector<ElseIf>& elseIfs,BlockStmt* elseBlock):
    expr_(expr),thenblock_(block),elseIfs_(elseIfs),elseBlock_(elseBlock){}

    NodeKind kind() override{
        return NodeKind::IfStmt;
    }
};

struct CallStmt : Stmt
{
    CallExpr* call_;
    explicit CallStmt(CallExpr* call): call_(call){}
    NodeKind kind() override{
        return NodeKind::CallStmt;
    }
};

struct PrintArg
{
    bool isString_;

    std::string string_;

    Expr* expr_;
};

struct PrintStmt : Stmt
{
    bool isPrintLn_;

    std::vector<PrintArg> args_;

    PrintStmt(bool isPrintLn,const std::vector<PrintArg>& args): isPrintLn_(isPrintLn),args_(args){}

    NodeKind kind() override{
        return NodeKind::PrintStmt;
    }
};