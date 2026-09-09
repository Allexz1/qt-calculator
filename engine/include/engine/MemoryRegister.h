#pragma once

namespace engine {

// A single-value calculator memory register (MC/MR/M+/M- semantics).
class MemoryRegister {
public:
    void clear();
    void add(double value);
    void subtract(double value);
    double recall() const;

private:
    double m_value = 0.0;
};

} // namespace engine
