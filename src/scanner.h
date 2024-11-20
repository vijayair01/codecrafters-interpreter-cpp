#ifndef SCANNER_H
#define SCANNER_H

#include "consts.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

namespace lox{
class Token{
  public:
    Token(std::string lexeme, TokenType type) : lexeme(lexeme), type(type) {
      std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
      identifier = lexeme;
      if(tokenident.find(type) != tokenident.end())
      {
          identifier = tokenident.at(type);
      } else {
          std::transform(
              identifier.begin(), identifier.end(), identifier.begin(), ::toupper);
      }
    }
    friend std::ostream &operator<<(std::ostream &os, const Token &token);
  private:
    std::string identifier;
    std::string lexeme;
    TokenType type;
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
    void add_token(const std::string &str, TokenType type);
    char advance();
    void scanChar();

    std::vector<Token> tokens;
    std::string p_file_contents;
    int current = 0;
    int _start = 0;
    int _end = 0;
    int line_number = 1;
};
} // namespace lox

#endif // SCANNER_H