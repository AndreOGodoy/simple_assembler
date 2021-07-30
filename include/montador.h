#ifndef MONTADOR_H
#define MONTADOR_H

#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "codes.h"

// Assembler é um namespace composto por um Lexer e um Parser
namespace Assembler {
using LINE_TOKEN = std::string;
using TOKEN = std::string;

class Lexer {
private:
  // Armazena as linhas válidas de código do arquivo original, já filtradas
  std::vector<LINE_TOKEN> buffer;

  // O gerenciamento de estados ocorre por meio destes dois inteiros
  // A próxima LINE_TOKEN fornecida é buffer[current_line]
  // Caso number_lines == current_line, o buffer chegou ao fim
  // O estado é reiniciado com current_line = 0
  size_t number_lines;
  size_t current_line;

public:
  Lexer(const std::string& file_name);
  Lexer(const Lexer &);

  // Método responsável por alimentar o Lexer com a próxima linha de código fonte
  LINE_TOKEN next();

  void reset_state();

  // Método responsável por separar tokens de uma linha de tokens
  static std::vector<TOKEN> tokenize_line(const LINE_TOKEN& line_token);
};

class Parser {
private:
  Lexer lexer; // O fluxo Lexer -> Parser é criado por meio de composição de objetos
  std::map<TOKEN, std::string> symbol_table;

public:
  Parser(Lexer &lexer);

  // Equivalente à primeira passagem: preenche a symbol table com os símbolos
  // do código fonte, e os provê uma tradução adequada.
  void parse();

  // Equivalente à segunda passagem: gera código alvo de acordo com os símbolos
  // da symbol table.
  std::string generate();

  // Pela semântica da linguagem de domínio, um token pode apenas possuir
  // uma dentre quatro funções: operador, registrador, label ou literal.
  static bool is_operator(const TOKEN &token);
  static bool is_register(const TOKEN &token);
  static bool is_label(const TOKEN &token);
  static bool is_literal(const TOKEN &token);

  // Atualiza ou insere um par chave-valor na symbol table.
  void upsert(const TOKEN& token, const std::string& value);
};
} // namespace Assembler
#endif
