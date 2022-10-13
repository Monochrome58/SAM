#include "parser.hpp"
#include "ctype.h"

using namespace parse;

inline bool str_is_alnum(const std::string &candidate) {
    auto i = candidate.begin();
    for (i; (i != candidate.end()) && (std::isalnum(*i)); ++i);
    return (i == candidate.end());
}

inline bool str_is_num(const std::string &candidate) {
    auto i = candidate.begin();
    for (i; (i != candidate.end()) && (std::isdigit(*i)); ++i);
    return (i == candidate.end());
}

inline bool is_terminal(const char &candidate) {
    return (
        (candidate == '\n') || (candidate == ' ')
        || (candidate == '\t')
    );
}

inline void Tokenizer::next() {
    source >> std::noskipws >> current_char;
}

inline void Tokenizer::consume() {
    current_token->value += current_char;
    next();
}

inline void Tokenizer::push_token(TokenContainer *&container) {
    container->push_back(current_token);
    current_token = new Token;
}

TokenContainer* Tokenizer::tokenize() {
    next();
    TokenContainer *tokens = new TokenContainer;
    current_token = new Token;
    while (source) {
        switch (current_char) {
            case ';':
                // Marks a comment; skip the remaining line
                while (current_char != '\n') next();
                break;
            case '"':
                current_token->line = line;
                current_token->type = TOKEN::STRING_T;
                next();
                while (current_char != '"') consume();
                push_token(tokens);
                next();
                break;
            case '.':
                current_token->line = line;
                current_token->type = TOKEN::DIRECTIVE_T;
                next();
                while (!is_terminal(current_char)) consume();
                push_token(tokens);
                break;
            case '\n':
                ++line;
            case '\r': case '\t': case ' ':
                next();
                break;
            default:
                current_token->line = line;
                while (!is_terminal(current_char)) consume();
                if (str_is_alnum(current_token->value)) {
                    if (is_opcode(current_token->value))
                        current_token->type = TOKEN::OPCODE_T;
                    else
                        current_token->type = TOKEN::PLAIN_T;
                } else if (str_is_num)
                    current_token->type = TOKEN::NUMERIC_T;
                push_token(tokens);
                next();
                break;
        }
    }
    return tokens;
}