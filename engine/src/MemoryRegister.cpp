#include "engine/MemoryRegister.h"

namespace engine {

void MemoryRegister::clear() {
    m_value = 0.0;
}

void MemoryRegister::add(double value) {
    m_value += value;
}

void MemoryRegister::subtract(double value) {
    m_value -= value;
}

double MemoryRegister::recall() const {
    return m_value;
}

} // namespace engine
