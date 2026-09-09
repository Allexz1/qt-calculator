#pragma once

#include <string>

#include "engine/AngleMode.h"
#include "engine/MemoryRegister.h"

namespace engine {

struct EvaluationResult {
    bool success = false;
    double value = 0.0;
    std::string errorMessage;
};

// Public facade for the calculator engine. This is the only class the UI
// layer needs to talk to; it owns no Qt dependency and no global state.
class CalculatorEngine {
public:
    EvaluationResult evaluate(const std::string& expression);

    void setAngleMode(AngleMode mode);
    AngleMode angleMode() const;

    void memoryClear();
    double memoryRecall() const;
    void memoryAdd(double value);
    void memorySubtract(double value);

private:
    AngleMode m_angleMode = AngleMode::Degrees;
    MemoryRegister m_memory;
};

} // namespace engine
