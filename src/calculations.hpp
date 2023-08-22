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
    double operator()(double first, double second = std::numeric_limits<double>::max()) const {
        bool is_binary_operator = std::holds_alternative<binary_operator>(action_);

        if ((is_binary_operator && second == std::numeric_limits<double>::max()) ||
            (!is_binary_operator && second != std::numeric_limits<double>::max())) {
                throw std::logic_error("Invalid arguments quantity for this operator");
        }

        return (is_binary_operator) ? processBinaryOperator(first, second)
                                    : processMathFunction(first);
    }

    bool operator==(const Operator& compare) const {
        bool isSame = std::visit(
            [](auto&& arg1, auto&& arg2) {
                using T1 = std::decay_t<decltype(arg1)>;
                using T2 = std::decay_t<decltype(arg2)>;

                if constexpr (std::is_same_v<T1, T2>) {
                    return true;
                } else {
                    return false;
                }
            },
            action_, compare.action_);

        return isSame;
    }

    bool operator!=(const Operator& compare) const { return !operator==(compare); }
};

inline const std::unordered_map<char, Operator> default_algebra_function_rules = {
    {'s',
     calculations::Operator(std::function<double(double)>([](double a) { return std::sin(a); }))},
    {'c',
     calculations::Operator(std::function<double(double)>([](double a) { return std::cos(a); }))},
    {'t',
     calculations::Operator(std::function<double(double)>([](double a) { return std::tan(a); }))},
    {'S',
     calculations::Operator(std::function<double(double)>([](double a) { return std::asin(a); }))},
    {'C',
     calculations::Operator(std::function<double(double)>([](double a) { return std::acos(a); }))},
    {'T',
     calculations::Operator(std::function<double(double)>([](double a) { return std::atan(a); }))},
    {'q',
     calculations::Operator(std::function<double(double)>([](double a) { return std::sqrt(a); }))},
    {'l',
     calculations::Operator(std::function<double(double)>([](double a) { return std::log10(a); }))},
    {'L',
     calculations::Operator(std::function<double(double)>([](double a) { return std::log(a); }))}};

inline const std::unordered_map<char, Operator> default_algebra_rules = {
    {'+', calculations::Operator(
              std::function<double(double, double)>([](double a, double b) { return a + b; }))},
    {'-', calculations::Operator(
              std::function<double(double, double)>([](double a, double b) { return a - b; }))},
    {'*', calculations::Operator(
              std::function<double(double, double)>([](double a, double b) { return a * b; }))},
    {'/', calculations::Operator(
              std::function<double(double, double)>([](double a, double b) { return a / b; }))},
    {'%', calculations::Operator(std::function<double(double, double)>(
              [](double a, double b) { return static_cast<int>(a) % static_cast<int>(b); }))},
    {'^', calculations::Operator(std::function<double(double, double)>(
              [](double a, double b) { return std::pow(a, b); }))},

};

class IAlgebra {
protected:
    std::unordered_map<char, Operator> rules_ = default_algebra_function_rules;

private:
    virtual void initializeRules(const std::unordered_map<char, Operator>& rules){};

public:
    IAlgebra() = default;

    void initializeRulesInterface(
        const std::unordered_map<char, Operator>& rules = default_algebra_rules) {
        if (!rules.empty() && rules != default_algebra_function_rules) {
            initializeRules(rules);
        }
    }

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

    virtual ~IAlgebra() {}
};

class ClassicAlgebra : public IAlgebra {
private:
    void initializeRules(const std::unordered_map<char, Operator>& rules) override {
        rules_.insert(default_algebra_rules.begin(), default_algebra_rules.end());
    }
};
}  // namespace calculations

#endif