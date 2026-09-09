#include "engine/Tokenizer.h"

#include <cctype>

#include "engine/CalculatorError.h"

namespace engine {

Tokenizer::Tokenizer(const std::string& input) : m_input(input) {}

namespace {
bool isDecimalPoint(char c) {
    return c == '.' || c == ',';
}
} // namespace

Token Tokenizer::readNumber() {
    std::size_t start = m_pos;
    bool sawDot = false;
    while (m_pos < m_input.size()) {
        char c = m_input[m_pos];
        if (std::isdigit(static_cast<unsigned char>(c))) {
            ++m_pos;
        } else if (isDecimalPoint(c) && !sawDot) {
            sawDot = true;
            ++m_pos;
        } else {
            break;
        }
    }
    std::string text = m_input.substr(start, m_pos - start);
    // std::stod always expects '.' as the decimal point regardless of the
    // active C locale, so normalize a ',' typed as a decimal separator.
    std::string normalized = text;
    for (char& c : normalized) {
        if (c == ',') c = '.';
    }
    Token token;
    token.type = TokenType::Number;
    token.text = text;
    try {
        token.numberValue = std::stod(normalized);
    } catch (const std::exception&) {
        throw CalculatorError("Invalid number: " + text);
    }
    return token;
}

Token Tokenizer::readIdentifier() {
    std::size_t start = m_pos;
    while (m_pos < m_input.size() &&
           (std::isalpha(static_cast<unsigned char>(m_input[m_pos])) ||
            std::isdigit(static_cast<unsigned char>(m_input[m_pos])))) {
        ++m_pos;
    }
    Token token;
    token.type = TokenType::Identifier;
    token.text = m_input.substr(start, m_pos - start);
    return token;
}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;

    while (m_pos < m_input.size()) {
        char c = m_input[m_pos];

        if (std::isspace(static_cast<unsigned char>(c))) {
            ++m_pos;
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(c)) || isDecimalPoint(c)) {
            tokens.push_back(readNumber());
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(c))) {
            tokens.push_back(readIdentifier());
            continue;
        }

        Token token;
        token.text = std::string(1, c);
        switch (c) {
            case '+': token.type = TokenType::Plus; break;
            case '-': token.type = TokenType::Minus; break;
            case '*': token.type = TokenType::Star; break;
            case '/': token.type = TokenType::Slash; break;
            case '^': token.type = TokenType::Caret; break;
            case '!': token.type = TokenType::Bang; break;
            case '%': token.type = TokenType::Percent; break;
            case '(': token.type = TokenType::LParen; break;
            case ')': token.type = TokenType::RParen; break;
            default:
                throw CalculatorError(std::string("Unexpected character '") + c + "'");
        }
        tokens.push_back(token);
        ++m_pos;
    }

    Token end;
    end.type = TokenType::End;
    tokens.push_back(end);
    return tokens;
}

} // namespace engine
