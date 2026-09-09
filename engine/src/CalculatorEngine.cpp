#include "engine/CalculatorEngine.h"

#include <cmath>

#include "engine/CalculatorError.h"
#include "engine/Parser.h"
#include "engine/Tokenizer.h"

namespace engine {

EvaluationResult CalculatorEngine::evaluate(const std::string& expression) {
    try {
        Tokenizer tokenizer(expression);
        std::vector<Token> tokens = tokenizer.tokenize();

        Parser parser(tokens, m_angleMode);
        double value = parser.parse();

        if (!std::isfinite(value)) {
            return EvaluationResult{false, 0.0, "Overflow"};
        }

        return EvaluationResult{true, value, ""};
    } catch (const CalculatorError& error) {
        return EvaluationResult{false, 0.0, error.what()};
    }
}

void CalculatorEngine::setAngleMode(AngleMode mode) {
    m_angleMode = mode;
}

AngleMode CalculatorEngine::angleMode() const {
    return m_angleMode;
}

void CalculatorEngine::memoryClear() {
    m_memory.clear();
}

double CalculatorEngine::memoryRecall() const {
    return m_memory.recall();
}

void CalculatorEngine::memoryAdd(double value) {
    m_memory.add(value);
}

void CalculatorEngine::memorySubtract(double value) {
    m_memory.subtract(value);
}

} // namespace engine
