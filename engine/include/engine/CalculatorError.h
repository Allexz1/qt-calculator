#pragma once

#include <stdexcept>
#include <string>

namespace engine {

class CalculatorError : public std::runtime_error {
public:
    explicit CalculatorError(const std::string& message)
        : std::runtime_error(message) {}
};

} // namespace engine
