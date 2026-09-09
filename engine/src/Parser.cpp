#include "engine/Parser.h"

#include "engine/CalculatorError.h"
#include "engine/MathConstants.h"
#include "engine/MathFunctions.h"

namespace engine {

Parser::Parser(const std::vector<Token>& tokens, AngleMode angleMode)
    : m_tokens(tokens), m_angleMode(angleMode) {}

const Token& Parser::current() const {
    return m_tokens[m_pos];
}

void Parser::advance() {
    if (m_pos + 1 < m_tokens.size()) {
        ++m_pos;
    }
}

bool Parser::check(TokenType type) const {
    return current().type == type;
}

void Parser::expect(TokenType type, const char* errorMessage) {
    if (!check(type)) {
        throw CalculatorError(errorMessage);
    }
    advance();
}

double Parser::parse() {
    if (check(TokenType::End)) {
        throw CalculatorError("Empty expression");
    }
    double result = parseExpression();
    if (!check(TokenType::End)) {
        throw CalculatorError("Unexpected token near '" + current().text + "'");
    }
    return result;
}

double Parser::parseExpression() {
    double value = parseTerm();
    while (check(TokenType::Plus) || check(TokenType::Minus)) {
        bool isPlus = check(TokenType::Plus);
        advance();
        double rhs = parseTerm();
        value = isPlus ? value + rhs : value - rhs;
    }
    return value;
}

double Parser::parseTerm() {
    double value = parseUnary();
    while (check(TokenType::Star) || check(TokenType::Slash)) {
        bool isMul = check(TokenType::Star);
        advance();
        double rhs = parseUnary();
        if (isMul) {
            value = value * rhs;
        } else {
            if (rhs == 0.0) {
                throw CalculatorError("Division by zero");
            }
            value = value / rhs;
        }
    }
    return value;
}

double Parser::parseUnary() {
    if (check(TokenType::Minus)) {
        advance();
        return -parseUnary();
    }
    if (check(TokenType::Plus)) {
        advance();
        return parseUnary();
    }
    return parsePower();
}

double Parser::parsePower() {
    double base = parsePostfix();
    if (check(TokenType::Caret)) {
        advance();
        double exponent = parseUnary();
        return mathfn::power(base, exponent);
    }
    return base;
}

double Parser::parsePostfix() {
    double value = parsePrimary();
    while (check(TokenType::Bang) || check(TokenType::Percent)) {
        if (check(TokenType::Bang)) {
            advance();
            value = mathfn::factorial(value);
        } else {
            advance();
            value = value / 100.0;
        }
    }
    return value;
}

double Parser::parseFunctionCall(const std::string& name) {
    expect(TokenType::LParen, "Expected '(' after function name");
    double arg = parseExpression();
    expect(TokenType::RParen, "Mismatched parentheses");

    if (name == "sqrt") return mathfn::squareRoot(arg);
    if (name == "sin") return mathfn::sinOf(arg, m_angleMode);
    if (name == "cos") return mathfn::cosOf(arg, m_angleMode);
    if (name == "tan") return mathfn::tanOf(arg, m_angleMode);
    if (name == "asin") return mathfn::asinOf(arg, m_angleMode);
    if (name == "acos") return mathfn::acosOf(arg, m_angleMode);
    if (name == "atan") return mathfn::atanOf(arg, m_angleMode);
    if (name == "log10") return mathfn::log10Of(arg);
    if (name == "ln") return mathfn::lnOf(arg);
    if (name == "exp") return mathfn::expOf(arg);

    throw CalculatorError("Unknown function: " + name);
}

double Parser::parsePrimary() {
    if (check(TokenType::Number)) {
        double value = current().numberValue;
        advance();
        return value;
    }

    if (check(TokenType::LParen)) {
        advance();
        double value = parseExpression();
        expect(TokenType::RParen, "Mismatched parentheses");
        return value;
    }

    if (check(TokenType::Identifier)) {
        std::string name = current().text;
        advance();

        if (check(TokenType::LParen)) {
            return parseFunctionCall(name);
        }

        if (name == "pi") return kPi;
        if (name == "e") return kE;

        throw CalculatorError("Unknown identifier: " + name);
    }

    if (check(TokenType::End)) {
        throw CalculatorError("Unexpected end of expression");
    }
    throw CalculatorError("Unexpected token near '" + current().text + "'");
}

} // namespace engine
