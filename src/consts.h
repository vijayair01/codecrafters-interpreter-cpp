#ifndef CONSTS_H
#define CONSTS_H

#include <map>
#include <string>

namespace lox {
enum TokenType
{
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,
    EQUAL,
    EQUAL_EQUAL,
    BANG,
    BANG_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    STRING,
    NUMBER,
    AND,
    CLASS,
    END_OF_FILE
};

// token type and identifier
const std::map<TokenType, std::pair<std::string, std::string>> tokenident = {
    {TokenType::LEFT_PAREN, {"LEFT_PAREN", "("}},
    {TokenType::RIGHT_PAREN, {"RIGHT_PAREN", ")"}},
    {TokenType::LEFT_BRACE, {"LEFT_BRACE", "{"}},
    {TokenType::RIGHT_BRACE, {"RIGHT_BRACE", "}"}},
    {TokenType::COMMA, {"COMMA", ","}},
    {TokenType::DOT, {"DOT", "."}},
    {TokenType::MINUS, {"MINUS", "-"}},
    {TokenType::PLUS, {"PLUS", "+"}},
    {TokenType::SEMICOLON, {"SEMICOLON", ";"}},
    {TokenType::SLASH, {"SLASH", "/"}},
    {TokenType::STAR, {"STAR", "*"}},
    {TokenType::EQUAL, {"EQUAL", "="}},
    {TokenType::EQUAL_EQUAL, {"EQUAL_EQUAL", "=="}},
    {TokenType::BANG, {"BANG", "!"}},
    {TokenType::BANG_EQUAL, {"BANG_EQUAL", "!="}},
    {TokenType::LESS, {"LESS", "<"}},
    {TokenType::LESS_EQUAL, {"LESS_EQUAL", "<="}},
    {TokenType::GREATER, {"GREATER", ">"}},
    {TokenType::GREATER_EQUAL, {"GREATER_EQUAL", ">="}},
    {TokenType::END_OF_FILE, {"EOF", ""}}};

const std::map<std::string, TokenType> reservedkeywords = {
    {"and", TokenType::AND},
    {"class", TokenType::CLASS}};

} // namespace lox

#endif // CONSTS_H
