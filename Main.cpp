#include <iostream>
#include <fstream>
#include "src/lexer/Lexer.hpp"
#include "src/parser/Parser.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: ./gol <archivo.gol>" << std::endl;
        return 1;
    }

    std::fstream archivo(argv[1]);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir: " << argv[1] << std::endl;
        return 1;
    }

    try {
        Lexer lexer(archivo);
        Parser parser(lexer);
        parser.parse();
        std::cout << "Sintaxis correcta" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}