#pragma once

#include <vector>

#include "engine/AngleMode.h"
#include "engine/Token.h"

namespace engine {

// Recursive-descent expression parser that evaluates as it parses (no
// separate AST). Grammar, highest to lowest binding:
//   expression := term (('+'|'-') term)*
//   term       := unary (('*'|'/') unary)*
//   unary      := ('-'|'+') unary | power
//   power      := postfix ('^' unary)?          (right-associative)
//   postfix    := primary ('!' | '%')*
//   primary    := NUMBER | IDENTIFIER '(' expression ')' | IDENTIFIER | '(' expression ')'
// Throws CalculatorError on any syntax or domain error.
class Parser {
public:
    Parser(const std::vector<Token>& tokens, AngleMode angleMode);

    double parse();

private:
    double parseExpression();
    double parseTerm();
    double parseUnary();
    double parsePower();
    double parsePostfix();
    double parsePrimary();
    double parseFunctionCall(const std::string& name);

    const Token& current() const;
    void advance();
    bool check(TokenType type) const;
    void expect(TokenType type, const char* errorMessage);

    const std::vector<Token>& m_tokens;
    std::size_t m_pos = 0;
    AngleMode m_angleMode;
};

} // namespace engine
