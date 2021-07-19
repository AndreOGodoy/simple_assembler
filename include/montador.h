#ifndef MONTADOR_H
#define MONTADOR_H

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>

#include <iostream>

const static std::map<std::string, std::string> _REGISTERS = {
        {"R0", "0"},
        {"R1", "1"},
        {"R2", "2"},
        {"R3", "3"},
};

const static std::map<std::string, std::string> _CODES = {
        { "HALT", "0"},
        { "LOAD", "1"},
        { "STORE", "2"},
        { "READ", "3"},
        { "WRITE", "4"},
        { "COPY", "5"},
        { "PUSH", "6"},
        { "POP", "7"},
        { "ADD", "8"},
        { "SUB", "9"},
        { "MUL", "10"},
        { "DIV", "11"},
        { "MOD", "12"},
        { "AND", "13"},
        { "OR", "14"},
        { "NOT", "15"},
        { "JUMP", "16"},
        { "JZ", "17"},
        { "JN", "18"},
        { "CALL", "19"},
        { "RET", "20"},
        {"END", "END"},
        {"WORD", "WORD"},
};


// TODO: structs, classes, e outras definições.
using LINE_TOKEN = std::string;
using TOKEN = std::string;

class Lexer {
private:
    std::vector<std::string> buffer;
    size_t number_lines;
    size_t current_line;
public:
    Lexer(std::string file_name);
    Lexer(const Lexer&);
    LINE_TOKEN next();

    void reset_state();

    std::vector<TOKEN> tokenize_line(LINE_TOKEN line_token);
};

class Parser {
private:
    Lexer lexer;
    std::map<std::string, std::string> symbol_table;
public:
    Parser(Lexer &lexer);
    void parse();

    static bool is_literal(const TOKEN& token);
    static bool is_label(const TOKEN& token);
    static bool is_register(const TOKEN& token);
    static bool is_operator(const TOKEN& token);

    void upsert(TOKEN token, std::string value);
};

#endif
