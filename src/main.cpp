#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "scanner.h"
#include "parser.h"

std::string read_file_contents(const std::string& filename);
using lox::Scanner;

void print_tokens(const std::vector<lox::Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << token << std::endl;
    }
}

void handle_tokenize(const std::string& file_contents) {
    if (!file_contents.empty()) {
        Scanner scanner(file_contents);
        auto tokens = scanner.get_tokens();
        print_tokens(tokens);
    } else {
        std::cout << "EOF  null" << std::endl;
    }
}

void handle_parse(const std::string& file_contents) {
    if (!file_contents.empty()) {
        std::vector<lox::Token> tokens;
        {
            Scanner scanner(file_contents);
            tokens = scanner.get_tokens();
        }
        if (!tokens.empty()) {
            lox::Parser parser(tokens);
            auto expr = parser.parse();
            if (expr) {
                std::cout << expr->form_string() << std::endl;
            } else {
                exit(65);
            }
        }
    } else {
        std::cout << "EOF  null" << std::endl;
    }
}

std::string get_evaluation_result(const lox::EvalResult& result) {
    std::stringstream ss;
    if (std::holds_alternative<double>(result)) {
        ss << std::get<double>(result);
    } else if (std::holds_alternative<bool>(result)) {
        ss << (std::get<bool>(result) == 0 ? "false" : "true");
    } else {
        ss << std::get<std::string>(result);
    }
    return ss.str();
}

std::string get_evaluation_result(const std::shared_ptr<lox::Expression>& expr) {
    if(!expr)
    {
        exit(65);
    }
    auto result = expr->evaluate();
    return get_evaluation_result(result);
}

std::string evaluate_expression(std::vector<lox::Token> tokens)
{
    if (!tokens.empty()) {
        lox::Parser parser(tokens);
        auto expr = parser.parse();
        if (expr) {
            auto result = expr->evaluate();
            return get_evaluation_result(result);
        } else {
            exit(65);
        }
    }
    return "";
}

void handle_evaluate(const std::string &file_contents)
{
    if(!file_contents.empty())
    {
        std::vector<lox::Token> tokens;
        {
            Scanner scanner(file_contents);
            tokens = scanner.get_tokens();
        }
        std::cout << evaluate_expression(tokens) << std::endl;
    }
}

void split_into_statements(const std::vector<lox::Token> &tokens, std::vector<std::vector<lox::Token>> &statements)
{

    for(size_t i = 0; i < tokens.size();)
    {
        std::vector<lox::Token> statement;
        while(tokens[i].type != lox::TokenType::SEMICOLON &&
              tokens[i].type != lox::TokenType::END_OF_FILE)
        {
            statement.push_back(tokens[i]);
            i++;
        }
        if(tokens[i].type == lox::TokenType::SEMICOLON ||
           tokens[i].type == lox::TokenType::END_OF_FILE)
        {
            i++;
        }
        if(statement.empty())
        {
            continue;
        }
        statement.push_back(lox::Token(lox::TokenType::END_OF_FILE, "EOF", "", 0));
        statements.push_back(statement);
    }
}

std::shared_ptr<lox::Expression> parse_expression(const std::vector<lox::Token>& tokens) {
    lox::Parser parser(tokens);
    return parser.parse();
}

void update_expression_with_variables(std::shared_ptr<lox::Expression>& expr, std::map<std::string, std::shared_ptr<lox::Expression>> &variables) {
    if (auto binary_expr = std::dynamic_pointer_cast<lox::Binary>(expr)) {
        update_expression_with_variables(binary_expr->left, variables);
        update_expression_with_variables(binary_expr->right, variables);
    } else if (auto unary_expr = std::dynamic_pointer_cast<lox::Unary>(expr)) {
        update_expression_with_variables(unary_expr->right, variables);
    } else if (auto grouping_expr = std::dynamic_pointer_cast<lox::Grouping>(expr)) {
        update_expression_with_variables(grouping_expr->expression, variables);
    } else if (auto var_expr = std::dynamic_pointer_cast<lox::Literal>(expr)) {
        if (variables.find(var_expr->value) != variables.end()) {
            expr = variables[var_expr->value];
        }
    }
}

void update_statement_with_variables(std::vector<lox::Token>& statement, std::map<std::string, std::shared_ptr<lox::Expression>> &variables) {
    for (auto& token : statement) {
        if (token.type == lox::TokenType::IDENTIFIER) {
            if (variables.find(token.lexeme) != variables.end()) {
                token.type = lox::TokenType::NUMBER;
                token.literal = get_evaluation_result(variables[token.lexeme]->evaluate());
            }
        }
    }
}

bool handle_multiple_assignments(
    std::vector<lox::Token>                                 &statement,
    std::map<std::string, std::shared_ptr<lox::Expression>> &variables)
{
    bool retVal = false;
    std::vector<std::vector<lox::Token>> statements;
    std::vector<lox::Token> temp;
    for(size_t i = 0; i < statement.size(); i++)
    {
        if(statement[i].type == lox::TokenType::EQUAL ||
           statement[i].type == lox::TokenType::END_OF_FILE)
        {
            if(statement[i].type == lox::TokenType::EQUAL)
            {
                retVal = true;
            }
            statements.push_back(temp);
            temp.clear();
        }
        else
        {
            temp.push_back(statement[i]);
        }
    }
    statements[statements.size() - 1].push_back(lox::Token(lox::TokenType::END_OF_FILE, "EOF", "", 0));
    update_statement_with_variables(statements[statements.size() - 1], variables);
     auto result =
        parse_expression(statements[statements.size() - 1]);
    statements.pop_back();
    for(auto statement : statements)
    {
        variables[statement[0].lexeme] = result;
    }
    statement = statements[0];
    return retVal;
}

void handle_statement(std::vector<lox::Token>& statement, std::map<std::string, std::shared_ptr<lox::Expression>>& variables) {
    bool print = false;
    bool isVar = false;
    if(statement[0].type == lox::TokenType::PRINT)
    {
        print = true;
        statement.erase(statement.begin());
    } else if(statement[0].type == lox::TokenType::VAR)
    {
        isVar = true;
        statement.erase(statement.begin());
    }
    auto                                 _statement = statement;
    std::vector<std::vector<lox::Token>> statements;
    if(statement.size() == 1 && statement[0].type == lox::TokenType::END_OF_FILE)
    {
        exit(65);
    }
    std::string                          var_name = statement[0].lexeme;
    if(statement.size() == 2 && statement[1].type == lox::TokenType::END_OF_FILE &&
       statement[0].type == lox::TokenType::IDENTIFIER)
    {
        if(isVar)
        {
            variables[statement[0].lexeme] = std::make_shared<lox::Literal>("nil");
            return;
        } else if(print)
        {
            std::cout << get_evaluation_result(variables[statement[0].lexeme]->evaluate()) << std::endl;
            return;
        }
    }
    if(handle_multiple_assignments(statement, variables))
    {
        if(print && statement.size() == 1)
        {
            std::cout << get_evaluation_result(variables[statement[0].literal]->evaluate())
                      << std::endl;
        }
    }
    else 
    {
        update_statement_with_variables(_statement, variables);
        auto expr = parse_expression(_statement);
        if(print)
        {
            std::cout << get_evaluation_result(expr->evaluate()) << std::endl;
        }
        else
        {
            //variables[var_name] = expr;
        }
    }

}

void handle_run(const std::string &file_contents) {
    std::map<std::string, std::shared_ptr<lox::Expression>> variables;
    if (!file_contents.empty()) {
        std::vector<lox::Token> tokens;
        {
            Scanner scanner(file_contents);
            tokens = scanner.get_tokens();
        }

        std::vector<std::vector<lox::Token>> statements;
        split_into_statements(tokens, statements);
        for (auto& statement : statements) {
            handle_statement(statement, variables);
        }
    }
}

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program <command> <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];
    std::string file_contents = read_file_contents(argv[2]);

    if (command == "tokenize") {
        handle_tokenize(file_contents);
    } else if (command == "parse") {
        handle_parse(file_contents);
    } else if (command == "evaluate") {
        handle_evaluate(file_contents);
    } else if (command == "run") {
        handle_run(file_contents);
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
