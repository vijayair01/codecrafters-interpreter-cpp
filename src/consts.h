#ifndef CONSTS_H
#define CONSTS_H

#include <map>
#include <string>

namespace lox {
enum TokenType
{
    LEFT_PAREN,
    RIGHT_PAREN,
    BANG_EQUAL,
    AND,
    CLASS,
    END_OF_FILE
};

// token type and laxeme
const std::map<TokenType, std::string> tokenlaxeme = {{TokenType::LEFT_PAREN, "("},
                                                      {TokenType::RIGHT_PAREN, ")"},
                                                      {TokenType::BANG_EQUAL, "!="},
                                                      {TokenType::AND, "and"},
                                                      {TokenType::CLASS, "class"},
                                                      {TokenType::END_OF_FILE, ""}};

// token type and identifier
const std::map<TokenType, std::string> tokenident = {{TokenType::LEFT_PAREN, "LEFT_PAREN"},
                                                     {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
                                                     {TokenType::BANG_EQUAL, "BANG_EQUAL"},
                                                     {TokenType::END_OF_FILE, "EOF"}};

} // namespace lox

#endif // CONSTS_H
