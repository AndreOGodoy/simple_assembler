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

    LINE_TOKEN token = this->buffer[current_line];
    this->current_line++;
    return token;
}

std::vector<TOKEN> Lexer::tokenize_line(LINE_TOKEN line_token) {
    std::stringstream ss(line_token);
    TOKEN item;

    std::vector<TOKEN> tokens;

    while (std::getline(ss, item, ' ')) {
        if (item == ";")
            break;

        tokens.push_back(item);
    }
    std::vector<TOKEN> filtered_tokens;
    std::copy_if(tokens.begin(), tokens.end(), std::back_inserter(filtered_tokens),
                 [](const TOKEN& x) {return !x.empty();});

    return filtered_tokens;
}

void Lexer::reset_state() {
    this->current_line = 0;
}

Parser::Parser(Lexer &lexer) : lexer(lexer) {}

void Parser::parse() {
    size_t i = 0;
    for (std::string line; line != "EOF$$$"; line = this->lexer.next(), i++) {
        auto tokens = this->lexer.tokenize_line(line);
        for (const auto& token : tokens) {
            if (is_label(token))
                this->upsert(token, std::to_string(i));

            else if (is_operator(token))
                this->upsert(token, _CODES.at(token));

            else if (is_register(token))
                this->upsert(token, _REGISTERS.at(token));

            else if (is_literal(token))
                this->upsert(token, token);
        }
    }
}

std::string Parser::generate() {
    this->lexer.reset_state();

    std::string header, output;
    size_t n_symbols = 0;
    bool end_program_flag = false;

    for (std::string line; line != "EOF$$$" && !end_program_flag; line = this->lexer.next()) {
        auto tokens = this->lexer.tokenize_line(line);
        for (const auto& token : tokens) {
            std::string code;

            if (is_label(token))
                continue;

            code = this->symbol_table[token];

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

bool Parser::is_register(const TOKEN& token) {
    return _REGISTERS.find(token) != _REGISTERS.end();
}

bool Parser::is_label(const TOKEN& token) {
    if (token.empty())
        return false;
    return token[token.size()-1] == ':';
}

bool Parser::is_literal(const TOKEN& token) {
    return std::all_of(token.begin(), token.end(), ::isdigit);
}

void Parser::upsert(TOKEN token, std::string value) {
    if (this->symbol_table.find(token) == this->symbol_table.end()) {
        symbol_table.insert(std::pair<std::string, std::string>(token, value));
    }
    else
        symbol_table[token] = value;
}

bool Parser::is_operator(const TOKEN &token) {
    return _CODES.find(token) != _CODES.end();
}
