#include <iostream>
#include "generator/generator.hpp"
#include "engine/engine.hpp"

int main(int argc, char *argv[]){
    if(argc > 1 && std::string(argv[1]) == "generator") {
        return main_generator(argc, argv);
    } else if(argc > 1 && std::string(argv[1]) == "engine"){
        return main_engine(argc, argv);
    } else {
        std::cout << "Erro: Parametros nao fornecidos" << std::endl << "\t.\program <generator|engine>" << std::endl;
    }

}