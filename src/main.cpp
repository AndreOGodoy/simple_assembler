#include <iostream>

#include "montador.h"

int main(int argc, char *argv[]) {
    // TODO: executar o montador...

    Lexer lexer(argv[1]);
    Parser parser(lexer);
    for (int i = 0; i < 5; i++) {
        parser.parse();
    }

    return 0;
}
