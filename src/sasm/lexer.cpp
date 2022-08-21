#include "lexer.hpp"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace lexer;

inline bool is_terminal(char character){
    return (character == ' ') || (character == '\n')
            || (character == '\r');
}

inline void Tokenizer::next(){
    source >> noskipws >> current_char;
}

Tokenizer::Tokenizer(char *source_name){
    source.open(source_name);
    next();
}

inline void Tokenizer::add_char_to_token(){
    tokens.concat_to_value(current_char);
    next();
}

int Tokenizer::tokenize(){
    while (source) {
        switch (current_char) {
            case ';':
                while (current_char != '\n') next();
                break;
            case '\"':
                tokens.set_type(TOKENS::STRING);
                tokens.set_line(line);
                next();
                while (current_char != '\"') add_char_to_token();
                next();
                tokens.push_token();
                break;
            case '\n':
                ++line;
            case '\r':
            case ' ':
                next();
                break;
            default:
                tokens.set_line(line);
                tokens.set_type(current_char);
                if (tokens.is_type(TOKENS::DIRECTIVE)) next();
                while (!is_terminal(current_char) && source) add_char_to_token();
                tokens.push_token();
                break;
        }
    }
    return 0;
}
