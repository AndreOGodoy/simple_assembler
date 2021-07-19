#ifndef MONTADOR_H
#define MONTADOR_H

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>

#include <iostream>

// TODO: structs, classes, e outras definições.
using LINE_TOKEN = std::string;
using TOKEN = std::string;

class Lexer {
private:
    std::vector<std::string> buffer;
    size_t number_lines;
public:
    Lexer(std::string file_name);
    Lexer(const Lexer&);
    LINE_TOKEN next();

    std::vector<TOKEN> tokenize_line(LINE_TOKEN line_token);
};

class Parser {
private:
    Lexer lexer;
    std::map<std::string, std::string> symbol_table;
public:
    Parser(Lexer &lexer);
    void parse();
};

#endif
