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
        return "\0";

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
    std::string line = this->lexer.next();

    auto tokens = this->lexer.tokenize_line(line);

}
