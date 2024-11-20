#include "scanner.h"

namespace lox {

void Scanner::add_token(const std::string& str, TokenType type)
{
    Token token(str, type);
    tokens.push_back(token);
}

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    os << token.identifier << " " << token.lexeme << " null";
    return os;
}

void Scanner::read_file()
{
    try {
        while (true) {
            scanChar();
        }
    } catch (const std::runtime_error &e) {
        //std::cerr << e.what() << std::endl;
    }
}

void Scanner::scanChar()
{
    char c = advance();
    switch (c)
    {
    case '(':
        add_token("(", TokenType::LEFT_PAREN);
        break;
    case ')':
        add_token(")", TokenType::RIGHT_PAREN);
        break;
    case '!':
        if (advance() == '=')
        {
            add_token("!=", TokenType::BANG_EQUAL);
            break;
        }
        current--;
        add_token("!", TokenType::BANG_EQUAL);
        break;
    default:
        break;
    }
}

char Scanner::advance()
{
    if(current >= p_file_contents.size())
    {
        add_token("", TokenType::END_OF_FILE);
        throw std::runtime_error("Reached EOF");
        return '\0';
    }
    return p_file_contents[current++];
}
} // namespace lox