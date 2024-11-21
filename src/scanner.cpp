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
    if(reservedkeywords.find(s) != reservedkeywords.end())
    {
        auto t = s;
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        std::string lexeme = "\"" + s + "\"";
        tokens.push_back(Token(reservedkeywords.at(s), s, t, line_number));
        return;
    }
    int count = std::count(s.begin(), s.end(), '.');
    if(std::find_if(s.begin(), s.end(), [](char &c) { return !isdigit(c) && c != '.'; }) ==
       s.end() && count <= 1)
    {
        std::string num = count == 1 ? s : s + ".0";
        tokens.push_back(Token(TokenType::NUMBER, s, num, line_number));
        return;
    }
    std::string lexeme = "\"" + s + "\"";
    tokens.push_back(Token(TokenType::STRING, lexeme, s, line_number));
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
    else if(token.type >= TokenType::AND && token.type <= TokenType::CLASS)
    {
        os << token.lexeme << " " << token.literal << " null";
    }
    else
    {
        os << "UNKNOWN " << token.lexeme << " " << token.literal;
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
        std::cerr << "[line " << line_number << "] Error: Unexpected character: " << c
                  << std::endl;
        error.set_retvalue(65);
        break;
    case '!':
    case '=':
    case '<':
    case '>':
        {
            std::string curr = std::string(1, c);
            if(peek() == '=')
            {
                curr += peek();
                add_token(curr);
                advance();
            }
            else
            {
                add_token(curr);
            }
        }
        break;
    case '/':
        if(peek() == '/')
        {
            while(peek() != '\n' && peek() != '\0')
            {
                advance();
            }
        }
        else
        {
            add_token(std::string(1, c));
        }
        break;
    break;
    case '\"':
        {
            char quote = c;
            std::string ans = "";
            while(peek() != quote && peek() != '\0')
            {
                ans += peek();
                if(peek() == '\n')
                {
                    line_number++;
                }
                advance();
            }
            if(peek() == '\0')
            {
                std::cerr << "[line " << line_number << "] Error: Unterminated string." << std::endl;
                error.set_retvalue(65);
            }
            else{
                add_token(ans);
                advance();
            }
        }
        break;
    case '\n':
        line_number++;
        break;
    case '\t':
    case ' ':
        break;
    default:
    {
        if(isalnum(c) || c == '.')
        {
            std::string ans = std::string(1, c);
            while(isalnum(peek()) || peek() == '.')
            {
                ans += peek();
                advance();
            }
            add_token(ans);
        }
    }
        break;
    }
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