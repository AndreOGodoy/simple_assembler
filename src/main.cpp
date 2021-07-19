#include <iostream>

#include "montador.h"

int main(int argc, char *argv[]) {
    // TODO: executar o montador...

    Lexer lexer(argv[1]);
    Parser parser(lexer);
    parser.parse();
    std::cout << parser.generate() << std::endl;
    return 0;
}
