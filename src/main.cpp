#include <iostream>

#include "montador.h"

int main(int argc, char *argv[]) {
    Assembler::Lexer lexer(argv[1]);
    Assembler::Parser parser(lexer);

    parser.parse(); // Primeira passagem
    auto output = parser.generate(); // Segunda passagem

    std::cout << output << std::endl;
    return 0;
}
