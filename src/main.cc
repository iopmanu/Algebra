#include "calculations.hpp"
#include "processor.hpp"
#include <iostream>

int main() {
    auto parser = preprocess::DjkstraProcessor();
    std::string expression = "1/2+(2+3)/(sin(9-2)^2-6/7)";
    // parser.inversePolishNotation(expression);
    for (auto x : parser.inversePolishNotation(expression)) {
        if (std::holds_alternative<preprocess::Token<double>>(x)) {
            std::cout << std::get<preprocess::Token<double>>(x).getData();
        } else if (std::holds_alternative<preprocess::Token<char>>(x)) {
            std::cout << std::get<preprocess::Token<char>>(x).getData();
        }
    }

    calculations::IAlgebra* base_algebra = new calculations::ClassicAlgebra();
    base_algebra->initializeRulesInterface();
    std::cout << std::endl << base_algebra->getRule('^')(2.14, 5.52);

    delete base_algebra;

    return 0;
}
