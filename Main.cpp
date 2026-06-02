#include<iostream>
#include<fstream>
#include "src/lexer/Lexer.hpp"

int main(int argc, char* argv[]){
    if(argc<2){
        std::cerr<<"Uso: ./gol <archive.gol>"<<std::endl;
        return 1;
    }

    std::fstream archivo(argv[1]);
    if(!archivo.is_open()){
        std::cerr<<"No se pudo abrir: "<<argv[1]<<std::endl;
        return 1;
    }

    Lexer lexer(archivo);
    Token t=lexer.nextToken();
    
    
   while (t.tokenId != TokenId::END_OF_FILE) {
    std::cout << t.text << "\t\t| línea " << t.line << std::endl;
    t = lexer.nextToken();
}
    return 0;

}