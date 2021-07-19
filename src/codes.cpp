#include "../include/codes.h"

namespace Assembler {
std::map<std::string, std::string> CODES::REGISTERS{
    {"R0", "0"},
    {"R1", "1"},
    {"R2", "2"},
    {"R3", "3"},
};

std::map<std::string, std::string> CODES::OPS{
    {"HALT", "0"},  {"LOAD", "1"},  {"STORE", "2"},   {"READ", "3"},
    {"WRITE", "4"}, {"COPY", "5"},  {"PUSH", "6"},    {"POP", "7"},
    {"ADD", "8"},   {"SUB", "9"},   {"MUL", "10"},    {"DIV", "11"},
    {"MOD", "12"},  {"AND", "13"},  {"OR", "14"},     {"NOT", "15"},
    {"JUMP", "16"}, {"JZ", "17"},   {"JN", "18"},     {"CALL", "19"},
    {"RET", "20"},  {"END", "END"}, {"WORD", "WORD"},
};
} // namespace Assembler