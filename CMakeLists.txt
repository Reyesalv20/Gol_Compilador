cmake_minimum_required(VERSION 3.20)
project(GolCompiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(gol
    Main.cpp
    src/lexer/Lexer.cpp
    src/parser/Parser.cpp
    src/semantic/SemanticAnalyzer.cpp
    src/semantic/SymbolTable.cpp
)