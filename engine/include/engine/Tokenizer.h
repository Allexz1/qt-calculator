#pragma once

#include <string>
#include <vector>

#include "engine/Token.h"

namespace engine {

// Converts an expression string into a token stream. Throws CalculatorError
// on any character it cannot recognize. Does not evaluate anything.
class Tokenizer {
public:
    explicit Tokenizer(const std::string& input);

    std::vector<Token> tokenize();

private:
    Token readNumber();
    Token readIdentifier();

    std::string m_input;
    std::size_t m_pos = 0;
};

} // namespace engine
