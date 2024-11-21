#include "scanner.h"

namespace lox {

void Scanner::add_token(std::string s)
{
    for(auto tokeniter = tokenident.begin(); tokeniter != tokenident.end(); tokeniter++)
    {
        if(tokeniter->second.second == s)
        {
            tokens.push_back(Token(tokeniter->first, "", "", line_number));
            return;
        }
    }
}

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    if(tokenident.find(token.type) != tokenident.end())
    {
        os << tokenident.at(token.type).first << " " << tokenident.at(token.type).second << " null";
    }
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
    case ')':
    case '{':
    case '}':
    case ',':
    case '.':
    case '-':
    case '+':
    case ';':
    case '*':
    case '/':
        add_token(std::string(1, c));
        break;
    case '#':
    case '@':
    case '$':
    case '^':
    case '%':
        std::cerr << "[line " << line_number << "] Error: Unexpected character: " << c << std::endl;
        error.set_retvalue(65);
        break;
    case '!':
        if (match('='))
        {
            advance();
            add_token("!=");
        }
        else
        {
            add_token(std::string(1, c));
        }
        break;
    case '=':
        if (match('='))
        {
            advance();
            add_token("==");
        }
        else
        {
            add_token(std::string(1, c));
        }
        break;
    default:
        break;
    }
}

bool Scanner::match(char c)
{
    if (p_file_contents[current] == c)
    {
        current++;
        return true;
    }
    return false;
}

char Scanner::advance()
{
    if(current >= p_file_contents.size())
    {
        add_token("");
        throw std::runtime_error("Reached EOF");
        return '\0';
    }
    return p_file_contents[current++];
}

void ErrorInScanner::add_error(std::string s)
{
    msg.push_back(std::move(s));
}

ErrorInScanner::~ErrorInScanner()
{
    for(const auto &error: msg)
    {
        std::cerr << error << std::endl;
    }
    if(retvalue)
    {
        exit(retvalue);
    }
}

int ErrorInScanner::get_retvalue() const
{
    return retvalue;
}

void ErrorInScanner::set_retvalue(int value)
{
    retvalue = value;
}

} // namespace lox