#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#ifndef __PREPROCESS_HPP__
#define __PREPROCESS_HPP__

namespace preprocess {
namespace exceptions {
class InvalidFunctionException : public std::exception {
    char *message;

public:
    InvalidFunctionException(char *msg) : message(msg) {}

    char *what() const noexcept { return message; }
};

class BracketSequenceException : public std::exception {
    char *message;

public:
    BracketSequenceException(char *msg) : message(msg) {}

    char *what() const noexcept { return message; }
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

class DjkstraProcessor {
    using token_storage = std::vector<std::variant<Token<double>, Token<char>>>;
    using function_iterator = std::unordered_map<std::string, char>::const_iterator;

private:
    token_storage output_sequence;

    void tokenize_input(const std::string &input_sequence) {
        if (!output_sequence.empty()) output_sequence.clear();
        int16_t bracket_quantity = 0;

        for (int i = 0; i < input_sequence.size(); i++) {
            if (input_sequence[i] == ' ')
                continue;
            else if (input_sequence[i] == 'x')
                output_sequence.push_back(Token(input_sequence[i]));
            else if (std::isalpha(input_sequence[i]) || is_operator(input_sequence[i])) {
                bool exception_condition = true;
                auto it = available_operators.find(input_sequence[i]);
                if (exception_condition &= (it != available_operators.end())) {
                    output_sequence.push_back(Token(*it));
                    continue;
                }

                auto function = get_valid_function(i, input_sequence);
                exception_condition |= (function != available_functions.end());

                if (exception_condition) {
                    output_sequence.push_back(Token(function->second));
                    i += function->first.size() - 1;
                } else {
                    throw new exceptions::InvalidFunctionException(
                        const_cast<char *>("Invalid function or operator\n"));
                }
            } else if (input_sequence[i] == '(') {
                bracket_quantity++;
                output_sequence.push_back(Token(input_sequence[i]));
            } else if (input_sequence[i] == ')') {
                bracket_quantity--;
                output_sequence.push_back(Token(input_sequence[i]));
            } else if (std::isdigit(input_sequence[i])) {
                auto digit_with_length = number_and_length(input_sequence, i);
                output_sequence.push_back(Token(digit_with_length.first));
                i += digit_with_length.second - 1;
            }
        }

        if (bracket_quantity != 0)
            throw new exceptions::BracketSequenceException(
                const_cast<char *>("Invalid bracket sequence"));
    }

    function_iterator get_valid_function(std::size_t current,
                                         const std::string &sequence) const noexcept {
        function_iterator retval = available_functions.end();
        for (int j = 1;
             (j < 4) && (j < sequence.size() - current) && (retval == available_functions.end());
             j++) {
            retval = available_functions.find(sequence.substr(current, j));
        }

        return retval;
    }

    std::pair<double, std::size_t> number_and_length(std::string sequence, std::size_t current) {
        std::size_t counter = current;
        for (; (counter < sequence.size()) && (std::isdigit(sequence[counter])); counter++)
            ;
        auto number_slice = sequence.substr(current, counter - current);
        return std::pair<double, std::size_t>(std::stod(number_slice), current - counter);
    }

    bool is_operator(char symbol) {
        return (available_operators.find(symbol) != available_operators.end());
    }

public:
    DjkstraProcessor() = default;
    DjkstraProcessor(const DjkstraProcessor &src) = delete;
    DjkstraProcessor(DjkstraProcessor &&) = delete;
};
}  // namespace preprocess

#endif  // __PREPROCESS_HPP__