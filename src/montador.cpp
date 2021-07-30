#include "../include/montador.h"

#include <iostream>

namespace Assembler {
    Lexer::Lexer(const std::string &file_name) {
        std::ifstream fs(file_name);
        std::string line;
        while (std::getline(fs, line, '\n')) {
            auto first_non_space = line.find_first_not_of(" \t");

            // Linhas vazias ou que possuem apenas comentários são ignoradas
            if (line.empty() || first_non_space == std::string::npos || line.at(first_non_space) == ';')
                continue;

            this->buffer.push_back(line);
        }

        this->current_line = 0;
        this->number_lines = this->buffer.size();
    }

    Lexer::Lexer(const Lexer &lexer) {
        this->number_lines = lexer.number_lines;
        this->current_line = lexer.current_line;
        this->buffer = lexer.buffer;
    }

    LINE_TOKEN Lexer::next() {
        if (this->current_line == this->number_lines)
            return "EOF$$$";

        // Ao prover uma LINE_TOKEN, atualiza o valor de current_line para
        // gerar a LINE_TOKEN seguinte quando este método for chamado novamente
        LINE_TOKEN token = this->buffer[current_line];
        this->current_line++;
        return token;
    }

    std::vector<TOKEN> Lexer::tokenize_line(const LINE_TOKEN &line_token) {
        std::stringstream ss(line_token);
        TOKEN item;

        std::vector<TOKEN> tokens;

        while (std::getline(ss, item, ' ')) {
            // LINE_TOKEN que começa com ';' não precisam ser tokenizadas
            if (not item.empty() and item.at(0) == ';')
                break;
            else if (item.empty()) {
                continue;
            }
            tokens.push_back(item);
        }
        // Remoção de pseudo-tokens que contém apenas espaços
        std::vector<TOKEN> filtered_tokens;
        std::copy_if(tokens.begin(), tokens.end(),
                     std::back_inserter(filtered_tokens),
                     [](const TOKEN &x) { return !x.empty(); });


        return filtered_tokens;
    }

    void Lexer::reset_state() { this->current_line = 0; }

    Parser::Parser(Lexer &lexer) : lexer(lexer) {}

    void Parser::parse() {
        size_t i = 0; // Linhas passadas
        size_t n_symbols = 0; // Símbolos passados
        for (std::string line; line != "EOF$$$"; line = this->lexer.next(), i++) {
            auto tokens = this->lexer.tokenize_line(line);
            for (const auto &token : tokens) {
                if (is_label(token)) {
                    auto label = token;
                    label.pop_back(); // Remove ':' da label
                    this->upsert(label, std::to_string(n_symbols));
                    continue;
                }
                else if (is_operator(token)) {
                    this->upsert(token, CODES::OPS.at(token));
                }

                else if (is_register(token)) {
                    this->upsert(token, CODES::REGISTERS.at(token));
                }

                else if (is_literal(token)) {
                    this->upsert(token, token);
                }

                // WORD e END não contam para o número de símbolos, pois são pseudo-instruções.
                if (token != "WORD" and token != "END")
                    n_symbols++;
            }
        }
    }

    std::string Parser::generate() {
        this->lexer.reset_state();

        std::string header, output;
        size_t n_symbols = 0; // Símbolos passados
        bool end_program_flag = false; // Caso verdadeiro, não executa próxima iteração do laço

        size_t i = 0; // Linhas passadas
        for (std::string line; line != "EOF$$$" && !end_program_flag;
             line = this->lexer.next(), i++) {

            auto tokens = this->lexer.tokenize_line(line);

            for (const auto &token : tokens) {
                std::string code;

                if (is_label(token))
                    continue;

                if (not is_register(token) and not is_literal(token) and not is_operator(token)) {
                    // Token é uma label sendo utilizada como operando. Calcula-se seu valor relativo
                    int abs_pos = std::stoi(this->symbol_table[token]);
                    code = std::to_string(abs_pos - int(n_symbols) - 1);
                } else {
                    code = this->symbol_table[token];
                }

                if (code == "WORD") {
                    continue;
                }

                if (code == "END") {
                    output.append(" ");
                    end_program_flag = true;
                    break;
                }

                output.append(code + " ");
                n_symbols++;
            }
        }

        header.append("MV-EXE\n");
        header.append("\n");
        header.append(std::to_string(n_symbols) + " ");
        header.append(std::to_string(100) + " ");
        header.append(std::to_string(999) + " ");
        header.append(std::to_string(100) + " ");
        header.append("\n");
        header.append("\n");

        return header + output;
    }

    // Um TOKEN é um operador caso seja encontrado na tabela de operações
    bool Parser::is_operator(const TOKEN &token) {
        return CODES::OPS.find(token) != CODES::OPS.end();
    }

    // Um TOKEN é um registrador caso seja encontrado na tabela de registradores
    bool Parser::is_register(const TOKEN &token) {
        return CODES::REGISTERS.find(token) != CODES::REGISTERS.end();
    }

    // Um TOKEN é uma label caso possua ':' ao fim
    bool Parser::is_label(const TOKEN &token) {
        if (token.empty())
            return false;
        return token[token.size() - 1] == ':';
    }


    // Um TOKEN é um literal caso contenha apenas digitos
    bool Parser::is_literal(const TOKEN &token) {
        return std::all_of(token.begin(), token.end(), ::isdigit);
    }

    void Parser::upsert(const TOKEN &token, const std::string &value) {
        if (this->symbol_table.find(token) == this->symbol_table.end()) {
            symbol_table.insert(std::pair<std::string, std::string>(token, value));
        } else
            symbol_table[token] = value;
    }
} // namespace Assembler
