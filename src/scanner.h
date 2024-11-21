#ifndef SCANNER_H
#define SCANNER_H

#include "consts.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

namespace lox{

class ErrorInScanner
{
  public:
    ErrorInScanner() = default;
    void add_error(std::string s);
    ~ErrorInScanner();

    int  get_retvalue() const;
    void set_retvalue(int value);

  private:
    std::vector<std::string> msg;
    int                      retvalue = 0;
};

class Token{
    public:
        Token(TokenType type, std::string lexeme, std::string literal, int line) : type(type), lexeme(lexeme), literal(literal), line(line) {}
        friend std::ostream &operator<<(std::ostream &os, const Token &token);
    private:
        TokenType type;
        std::string lexeme;
        std::string literal;
        int line;
};

class Scanner {
  public:
    Scanner(const std::string &file_contents) : p_file_contents(file_contents) {}
    std::vector<Token>   get_tokens()
    {
        read_file();
        return tokens;
    }

  private:
    void read_file();
    void add_token(std::string s);
    char advance();
    void scanChar();
    char peek();

    std::vector<Token> tokens;
    std::string p_file_contents;
    int current = 0;
    int _start = 0;
    int _end = 0;
    int line_number = 1;
    ErrorInScanner error;
};
} // namespace lox

#endif // SCANNER_H