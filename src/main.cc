#include "processor.hpp"
#include <iostream>

int main() {
    auto parser = preprocess::DjkstraProcessor();
    std::string expression = "((1 + 2) + 3 + tan(5)) * atan(6) * sqrt(5) / 2";
    // parser.inversePolishNotation(expression);
    for (auto x : parser.inversePolishNotation(expression)) {
        if (std::holds_alternative<preprocess::Token<double>>(x)) {
            std::cout << std::get<preprocess::Token<double>>(x).getData();
        } else if (std::holds_alternative<preprocess::Token<char>>(x)) {
            std::cout << std::get<preprocess::Token<char>>(x).getData();
        }
    }

    return 0;
}
