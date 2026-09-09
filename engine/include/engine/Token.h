#pragma once

#include <string>

namespace engine {

enum class TokenType {
    Number,
    Identifier,
    Plus,
    Minus,
    Star,
    Slash,
    Caret,
    Bang,
    Percent,
    LParen,
    RParen,
    End
};

struct Token {
    TokenType type;
    double numberValue = 0.0;
    std::string text;
};

} // namespace engine
