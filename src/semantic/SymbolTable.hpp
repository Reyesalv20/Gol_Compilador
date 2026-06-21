#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Symbol.hpp"

class SymbolTable{

    private:
    std::vector<std::unordered_map<std::string, Symbol* >> scopes_;

    public:
    void enterScopes();
    void exitScopes();
    bool declare(Symbol* symbol);
    Symbol* lookup(const std::string& name);
};
