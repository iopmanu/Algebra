#include <any>
#include <functional>
#include <variant>

#include <unordered_map>

#ifndef __CALCULATIONS_HPP__
#define __CALCULATIONS_HPP__

namespace calculations {
using function = std::function<double(double)>;
using binary_operator = std::function<double(double, double)>;

class Operator {
public:
    std::variant<function, binary_operator> action_;

    Operator(std::variant<function, binary_operator> action) : action_(action) {}

    double processBinaryOperator(double first, double second) const {
        return std::get<binary_operator>(action_)(first, second);
    }

    double processMathFunction(double first) const { return std::get<function>(action_)(first); }

public:
    double operator()(double first, double second = 0) const {
        return (std::holds_alternative<binary_operator>(action_))
                   ? processBinaryOperator(first, second)
                   : processMathFunction(first);
    }
};

class IAlgebra {
protected:
    std::unordered_map<char, Operator> rules_;

private:
    virtual void initialize_rules() = 0;

public:
    IAlgebra() = default;

    Operator getRule(const char identifier) const {
        if (rules_.empty()) {
            throw std::logic_error("Undefined rules of created algebra\n");
        }

        auto rule = rules_.find(identifier);
        if (rule == rules_.end()) {
            throw std::logic_error("Invalid rule of created algebra\n");
        }
        return rule->second;
    }
};

class ClassicAlgebra : public IAlgebra {
    void initialize_rules() override {
        
    }
};
}

#endif