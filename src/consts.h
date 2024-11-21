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
    BANG,
    LESS,
    GREATER,
    EQUAL_EQUAL,
    BANG_EQUAL,
    LESS_EQUAL,
    GREATER_EQUAL,
    STRING,
    NUMBER,
    LITERAL,
    AND,
    CLASS,
    ELSE,
    FALSE,
    FOR,
    FUN,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,
    END_OF_FILE
};

// token type and identifier
const std::map<TokenType, std::pair<std::string, std::string>> tokenident = {
    {TokenType::LEFT_PAREN,         {"LEFT_PAREN",      "("}},
    {TokenType::RIGHT_PAREN,        {"RIGHT_PAREN",     ")"}},
    {TokenType::LEFT_BRACE,         {"LEFT_BRACE",      "{"}},
    {TokenType::RIGHT_BRACE,        {"RIGHT_BRACE",     "}"}},
    {TokenType::COMMA,              {"COMMA",           ","}},
    {TokenType::DOT,                {"DOT",             "."}},
    {TokenType::MINUS,              {"MINUS",           "-"}},
    {TokenType::PLUS,               {"PLUS",            "+"}},
    {TokenType::SEMICOLON,          {"SEMICOLON",       ";"}},
    {TokenType::SLASH,              {"SLASH",           "/"}},
    {TokenType::STAR,               {"STAR",            "*"}},
    {TokenType::EQUAL,              {"EQUAL",           "="}},
    {TokenType::BANG,               {"BANG",            "!"}},
    {TokenType::LESS,               {"LESS",            "<"}},
    {TokenType::GREATER,            {"GREATER",         ">"}},
    {TokenType::EQUAL_EQUAL,        {"EQUAL_EQUAL",     "=="}},
    {TokenType::BANG_EQUAL,         {"BANG_EQUAL",      "!="}},
    {TokenType::LESS_EQUAL,         {"LESS_EQUAL",      "<="}},
    {TokenType::GREATER_EQUAL,      {"GREATER_EQUAL",   ">="}},
    {TokenType::END_OF_FILE,        {"EOF",             ""}}};

const std::map<std::string, TokenType> reservedkeywords = {{"and", TokenType::AND},
                                                           {"class", TokenType::CLASS},
                                                           {"else", TokenType::ELSE},
                                                           {"false", TokenType::FALSE},
                                                           {"for", TokenType::FOR},
                                                           {"fun", TokenType::FUN},
                                                           {"if", TokenType::IF},
                                                           {"nil", TokenType::NIL},
                                                           {"or", TokenType::OR},
                                                           {"print", TokenType::PRINT},
                                                           {"return", TokenType::RETURN},
                                                           {"super", TokenType::SUPER},
                                                           {"this", TokenType::THIS},
                                                           {"true", TokenType::TRUE},
                                                           {"var", TokenType::VAR},
                                                           {"while", TokenType::WHILE}};

} // namespace lox

#endif // CONSTS_H
