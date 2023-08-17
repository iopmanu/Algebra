#include <iostream>
#include <variant>
#include <vector>

#include "processor.hpp"

int main() {
    std::vector<std::variant<int, std::string>> v;
    for (int i = 0; i < 2; i++) {
        v.push_back(1);
        v.push_back(std::string("LALALALA"));
    }

    for (auto x : v) {
        if (std::holds_alternative<int>(x)) {
            std::cout << std::get<int>(x) << std::endl;
        } else if (std::holds_alternative<std::string>(x)) {
            std::cout << std::get<std::string>(x) << std::endl;
        }
    }

    return 0;
}