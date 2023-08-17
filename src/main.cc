#include "processor.hpp"
#include <iostream>

int main() {
    auto parser = preprocess::DjkstraProcessor();
    std::string expression = "((1 + 2) + 3 + tan(5)) * atan(6) * sqrt(5) / 2";
    parser.inverse_polish_notation(expression);
    for (auto x : parser.output_sequence) {
        if (std::holds_alternative<preprocess::Token<double>>(x)) {
            std::cout << std::get<preprocess::Token<double>>(x).getData() << std::endl;
        } else if (std::holds_alternative<preprocess::Token<char>>(x)) {
            std::cout << std::get<preprocess::Token<char>>(x).getData() << std::endl;
        }
    }

    return 0;
}
