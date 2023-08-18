#include <iostream>
#include <variant>

#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifndef __PREPROCESS_HPP__
#define __PREPROCESS_HPP__

namespace preprocess {
namespace exceptions {
class InvalidFunctionException : public std::exception {
    const char *message;

public:
    InvalidFunctionException(const char *msg) : message(msg) {}

    const char *what() const noexcept { return message; }
};

class BracketSequenceException : public std::exception {
    const char *message;

public:
    BracketSequenceException(const char *msg) : message(msg) {}

    const char *what() const noexcept { return message; }
};
}  // namespace exceptions

template <typename T>
struct Token {
private:
    T token_data;

public:
    Token() noexcept {};
    Token(const T &src) : token_data(src){};

    const T &getData() noexcept { return token_data; }
    const T &setData(const T &src) { token_data = src; }
};

/// @brief  It can be usefull meta of the preprocessing part.
inline const std::unordered_map<std::string, char> available_functions = {
    {"sin", 's'},  {"cos", 'c'},  {"tan", 't'}, {"asin", 'S'}, {"acos", 'C'},
    {"atan", 'T'}, {"sqrt", 'q'}, {"log", 'l'}, {"ln", 'L'},   {"exp", 'e'}};

inline const std::unordered_set<char> available_operators = {'+', '-', '*', '/', '^'};

inline const std::unordered_map<char, int8_t> operators_priorities = {
    {'(', 0}, {')', 0}, {'+', 2}, {'-', 2}, {'*', 3}, {'/', 3}, {'%', 3}, {'^', 4}, {'s', 4},
    {'c', 4}, {'t', 4}, {'S', 4}, {'C', 4}, {'T', 4}, {'q', 4}, {'l', 4}, {'L', 4}, {'e', 4}};

class DjkstraProcessor {
    using token_storage = std::vector<std::variant<Token<double>, Token<char>>>;
    using function_iterator = std::unordered_map<std::string, char>::const_iterator;
    using tokens_order = std::stack<std::variant<Token<double>, Token<char>>>;

private:
    void tokenizeInput(const std::string &input_sequence) {
        if (!output_sequence.empty()) output_sequence.clear();
        int16_t bracket_quantity = 0;

        for (int i = 0; i < input_sequence.size(); i++) {
            if (input_sequence[i] == ' ')
                continue;
            else if (input_sequence[i] == 'x')
                output_sequence.push_back(Token<char>(input_sequence[i]));
            else if (std::isalpha(input_sequence[i]) || isOperator(input_sequence[i])) {
                bool exception_condition = true;
                auto it = available_operators.find(input_sequence[i]);
                if (exception_condition &= (it != available_operators.end())) {
                    output_sequence.push_back(Token<char>(*it));
                    continue;
                }

                auto function = getValidFunction(i, input_sequence);
                exception_condition |= (function != available_functions.end());

                if (exception_condition) {
                    output_sequence.push_back(Token<char>(function->second));
                    i += function->first.size() - 1;
                } else {
                    throw exceptions::InvalidFunctionException("Invalid function or operator\n");
                }
            } else if (input_sequence[i] == '(') {
                bracket_quantity++;
                output_sequence.push_back(Token<char>(input_sequence[i]));
            } else if (input_sequence[i] == ')') {
                bracket_quantity--;
                output_sequence.push_back(Token<char>(input_sequence[i]));
            } else if (std::isdigit(input_sequence[i])) {
                auto digit_with_length = numberAndLength(input_sequence, i);
                output_sequence.push_back(Token<double>(digit_with_length.first));
                i += digit_with_length.second - 1;
            }
        }

        if (bracket_quantity != 0)
            throw exceptions::BracketSequenceException("Invalid bracket sequence");
    }

    function_iterator getValidFunction(std::size_t current,
                                       const std::string &sequence) const noexcept {
        function_iterator retval = available_functions.end();
        for (int j = 1;
             (j <= 4) && (j < sequence.size() - current) && (retval == available_functions.end());
             j++) {
            retval = available_functions.find(sequence.substr(current, j));
        }

        return retval;
    }

    std::pair<double, std::size_t> numberAndLength(const std::string &sequence,
                                                   std::size_t current) const noexcept {
        std::size_t counter = current;
        for (; (counter < sequence.size()) && (std::isdigit(sequence[counter])); counter++)
            ;
        auto number_slice = sequence.substr(current, counter - current);
        return std::pair<double, std::size_t>(std::stod(number_slice), counter - current);
    }

    bool isOperator(char symbol) {
        return (available_operators.find(symbol) != available_operators.end());
    }

    void processBrackets(token_storage &inverse_notation, tokens_order &bracket_processor) {
        if (!bracket_processor.empty()) {
            auto token = std::get<Token<char>>(bracket_processor.top());
            bracket_processor.pop();

            while (token.getData() != '(') {
                inverse_notation.push_back(token);
                token = std::get<Token<char>>(bracket_processor.top());
                bracket_processor.pop();
            }
        }
    }

    void shiftTokens(token_storage &inverse_notation, tokens_order &order) {
        while (!order.empty()) {
            auto token = std::get<Token<char>>(order.top());
            order.pop();
            inverse_notation.push_back(token);
        }
    }

    int8_t priorityDifference(char first_operator, char second_operator) {
        return (operators_priorities.find(first_operator)->second -
                operators_priorities.find(second_operator)->second);
    }

public:
    token_storage output_sequence;

    DjkstraProcessor() = default;
    DjkstraProcessor(const DjkstraProcessor &src) = delete;
    DjkstraProcessor(DjkstraProcessor &&) = delete;

    token_storage inversePolishNotation(const std::string &input_sequence) {
        if (output_sequence.empty()) tokenizeInput(input_sequence);
        token_storage postfix_inverse_notation;
        tokens_order bracket_processor;

        for (auto token : output_sequence) {
            bool is_double_condition = std::holds_alternative<Token<double>>(token);
            auto token_data = (is_double_condition) ? std::get<Token<double>>(token).getData()
                                                    : std::get<Token<char>>(token).getData();

            if (is_double_condition) {
                postfix_inverse_notation.push_back(token);
            } else if (!is_double_condition && token_data == 'x') {
                postfix_inverse_notation.push_back(token);
            } else if (!is_double_condition && token_data == 'e') {
                postfix_inverse_notation.push_back(Token<double>(std::exp(1)));
            } else {
                if (token_data == '(') {
                    bracket_processor.push(token);
                } else if (token_data == ')') {
                    processBrackets(postfix_inverse_notation, bracket_processor);
                } else {
                    while (
                        !bracket_processor.empty() &&
                        priorityDifference(std::get<Token<char>>(bracket_processor.top()).getData(),
                                           token_data) >= 0) {
                        postfix_inverse_notation.push_back(
                            std::get<Token<char>>(bracket_processor.top()));
                        bracket_processor.pop();
                    }

                    bracket_processor.push(token);
                }
            }
        }
        shiftTokens(postfix_inverse_notation, bracket_processor);

        return postfix_inverse_notation;
    }
};
}  // namespace preprocess

#endif  // __PREPROCESS_HPP__