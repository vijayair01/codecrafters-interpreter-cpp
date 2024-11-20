#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "scanner.h"

std::string read_file_contents(const std::string& filename);
using lox::Scanner;

void print_tokens(const std::vector<lox::Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << token << std::endl;
    }
}

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);

        if (!file_contents.empty()) {
            Scanner scanner(file_contents);
            auto tokens = scanner.get_tokens();
            print_tokens(tokens);
        }
        else {
            std::cout << "EOF  null" << std::endl;
        }        
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
