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
        int                  line;
        std::string          lexeme;
        std::string          literal;
        TokenType            type;

      private:
};

class Scanner {
  public:
    Scanner(const std::string &file_contents) : p_file_contents(file_contents) {}
    std::vector<Token> get_tokens()
    {
        read_file();
        return tokens;
    }

  private:
    void read_file();
    void scanChar();
    void add_token(std::string s);
    bool add_existing_token(const std::string &s);
    bool add_reserved_keyword(const std::string &s);
    bool add_string_token(const std::string &s);
    bool add_number_token(std::string &s);
    void handle_unexpected_char(char c);
    void handle_two_char_token(char c);
    void handle_slash();
    void handle_string();
    void handle_default(char c);
    char peek();
    char advance();

    std::vector<Token> tokens;
    std::string p_file_contents;
    int current = 0;
    int _start = -1;
    int line_number = 1;
    ErrorInScanner error;
};
} // namespace lox

#endif // SCANNER_H