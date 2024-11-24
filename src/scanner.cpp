#include "scanner.h"

namespace lox {

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
    switch(c)
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
        add_token(std::string(1, c));
        break;
    case '#':
    case '@':
    case '$':
    case '^':
    case '%':
        handle_unexpected_char(c);
        break;
    case '!':
    case '=':
    case '<':
    case '>':
        handle_two_char_token(c);
        break;
    case '/':
        handle_slash();
        break;
    case '\"':
        handle_string();
        break;
    case '\n':
        line_number++;
        break;
    case '\t':
    case ' ':
        break;
    default:
        handle_default(c);
        break;
    }
}

void Scanner::add_token(std::string s)
{
    if (add_existing_token(s) || add_reserved_keyword(s) || add_string_token(s) || add_number_token(s)) {
        return;
    }
    if (!s.empty()) {
        tokens.push_back(Token(TokenType::IDENTIFIER, s, s, line_number));
    }
}

bool Scanner::add_existing_token(const std::string& s) {
    auto it = std::find_if(tokenident.begin(), tokenident.end(), [&s](const auto &p) { return p.second.second == s; });
    if(it != tokenident.end()) {
        tokens.push_back(Token(it->first, "", "", line_number));
        return true;
    }
    return false;
}

bool Scanner::add_reserved_keyword(const std::string& s) {
    if(reservedkeywords.find(s) != reservedkeywords.end()) {
        auto t = s;
        auto u = s;
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);
        std::transform(u.begin(), u.end(), u.begin(), ::toupper);
        tokens.push_back(Token(reservedkeywords.at(t), u, t, line_number));
        return true;
    }
    return false;
}

bool Scanner::add_string_token(const std::string& s) {
    if(s.size() > 1 && s[0] == '\"' && s[s.size() - 1] == '\"') {
        std::string lexeme  = s;
        std::string literal = s.substr(1, s.size() - 2);
        tokens.push_back(Token(TokenType::STRING, lexeme, literal, line_number));
        return true;
    }
    return false;
}

bool Scanner::add_number_token(std::string& s) {
    std::string t;
    int countdots = 0;
    while(!s.empty() && (isdigit(s[0]) || (countdots == 0 && s[0] == '.'))) {
        if(s[0] == '.') {
            countdots++;
        }
        t += s[0];
        s.erase(s.begin());
    }
    if(!t.empty()) {
        std::string num = t + ".0";
        if(countdots == 1) {
            num = t;
            auto it = std::find_if(num.rbegin(), num.rend(), [](char &c) { return c != '0'; });
            num.erase(it.base(), num.end());
            if(num[num.size() - 1] == '.') {
                num += "0";
            }
        }
        tokens.push_back(Token(TokenType::NUMBER, t, num, line_number));
    }
    return false;
}

void Scanner::handle_unexpected_char(char c) {
    std::cerr << "[line " << line_number << "] Error: Unexpected character: " << c << std::endl;
    error.set_retvalue(65);
}

void Scanner::handle_two_char_token(char c) {
    std::string curr(1, c);
    if(peek() == '=') {
        curr += advance();
    }
    add_token(curr);
}

void Scanner::handle_slash() {
    if(peek() == '/') {
        while(peek() != '\n' && peek() != '\0') {
            advance();
        }
    } else {
        add_token(std::string(1, '/'));
    }
}

void Scanner::handle_string() {
    std::string ans;
    while(peek() != '\"' && peek() != '\0') {
        if(peek() == '\n') line_number++;
        ans += advance();
    }
    if(peek() == '\0') {
        std::cerr << "[line " << line_number << "] Error: Unterminated string." << std::endl;
        error.set_retvalue(65);
    } else {
        advance();
        add_token("\"" + ans + "\"");
    }
}

void Scanner::handle_default(char c) {
    std::string ans(1, c);
    while (!isspace(peek()) && peek() != '\0' && (isalnum(peek()) || peek() == '.' || peek() == '_')) {
        ans += advance();
    }
    add_token(ans);
}

char Scanner::peek()
{
    return p_file_contents[current];
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

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    if(tokenident.find(token.type) != tokenident.end())
    {
        os << tokenident.at(token.type).first << " " << tokenident.at(token.type).second << " null";
    }
    else if(token.type == TokenType::STRING)
    {
        os << "STRING " << token.lexeme << " " << token.literal;
    }
    else if(token.type == TokenType::NUMBER)
    {
        os << "NUMBER " << token.lexeme << " " << token.literal;
    }
    else if(token.type >= TokenType::AND && token.type <= TokenType::WHILE)
    {
        os << token.lexeme << " " << token.literal << " null";
    }
    else
    {
        os << "IDENTIFIER " << token.lexeme << " null";
    }
    return os;
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
    if(retvalue !=0)
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