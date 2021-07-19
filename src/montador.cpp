#include "../include/montador.h"

// TODO: implementação do montador
Lexer::Lexer(std::string file_name) {
    std::ifstream fs(file_name);
    std::string line;

    while (std::getline(fs, line, '\n')) {
        if (line.size() == 0 || line[0] == ';' || line[0] == ' ')
            continue;

        this->buffer.push_back(line);
    }

    this->number_lines = this->buffer.size();
    for (auto item: this->buffer)
        std::cout << item << std::endl;

    std::cout << this->number_lines << std::endl;
}
