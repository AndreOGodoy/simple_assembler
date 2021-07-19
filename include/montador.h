#ifndef MONTADOR_H
#define MONTADOR_H

#include <vector>
#include <string>
#include <fstream>

#include <iostream>

// TODO: structs, classes, e outras definições.
using TOKEN = std::string;


class Lexer {
private:
    std::vector<std::string> buffer;
    size_t number_lines;
public:
    Lexer(std::string file_name);
    TOKEN next();
};

#endif
