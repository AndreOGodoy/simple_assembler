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
}

Lexer::Lexer(const Lexer &lexer) {
    this->number_lines = lexer.number_lines;
    this->buffer = lexer.buffer;
}

LINE_TOKEN Lexer::next() {
    if (this->buffer.empty())
        return "EOF$$$";

    LINE_TOKEN token = this->buffer[0];
    this->buffer.erase(this->buffer.begin());
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
