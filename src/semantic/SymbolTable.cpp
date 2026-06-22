#include "SymbolTable.hpp"

void SymbolTable::enterScopes(){
    scopes_.push_back({});
}

void SymbolTable::exitScopes(){
    if(!scopes_.empty()){
        scopes_.pop_back();
    }
}

bool SymbolTable::declare(Symbol* symbol){
    if(scopes_.empty()){
        enterScopes();
    }

    auto& currentScope=scopes_.back();

    if(currentScope.count(symbol->name_)){
        return false;
    }
    currentScope[symbol->name_]=symbol;
    return true;
}

Symbol* SymbolTable::lookup(const std::string& name){
    for(auto it=scopes_.rbegin();it!=scopes_.rend();it++){

        auto found=it->find(name);
        if(found!=it->end()){
            return found->second;
        }
    }
    return nullptr;
}