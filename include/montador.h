#ifndef MONTADOR_H
#define MONTADOR_H

#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "codes.h"

namespace Assembler {
using LINE_TOKEN = std::string;
using TOKEN = std::string;

class Lexer {
private:
  std::vector<std::string> buffer;
  size_t number_lines;
  size_t current_line;

public:
  Lexer(const std::string& file_name);
  Lexer(const Lexer &);
  LINE_TOKEN next();

  void reset_state();

  static std::vector<TOKEN> tokenize_line(const LINE_TOKEN& line_token);
};

class Parser {
private:
  Lexer lexer;
  std::map<std::string, std::string> symbol_table;

public:
  Parser(Lexer &lexer);
  void parse();
  std::string generate();

  static bool is_literal(const TOKEN &token);
  static bool is_label(const TOKEN &token);
  static bool is_register(const TOKEN &token);
  static bool is_operator(const TOKEN &token);

  void upsert(const TOKEN& token, const std::string& value);
};
} // namespace Assembler
#endif
