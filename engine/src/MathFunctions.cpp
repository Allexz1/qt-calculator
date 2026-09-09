#include "engine/MathFunctions.h"

#include <cmath>

#include "engine/CalculatorError.h"
#include "engine/MathConstants.h"

namespace engine::mathfn {

namespace {

double toRadians(double x, AngleMode mode) {
    return mode == AngleMode::Degrees ? x * kPi / 180.0 : x;
}

double fromRadians(double x, AngleMode mode) {
    return mode == AngleMode::Degrees ? x * 180.0 / kPi : x;
}

} // namespace

double squareRoot(double x) {
    if (x < 0.0) {
        throw CalculatorError("Cannot take square root of a negative number");
    }
    return std::sqrt(x);
}

double reciprocal(double x) {
    if (x == 0.0) {
        throw CalculatorError("Division by zero");
    }
    return 1.0 / x;
}

double power(double base, double exponent) {
    return std::pow(base, exponent);
}

double factorial(double x) {
    if (x < 0.0 || std::abs(x - std::round(x)) > 1e-9) {
        throw CalculatorError("Factorial requires a non-negative integer");
    }
    if (x > 170.0) {
        throw CalculatorError("Overflow");
    }
    long long n = static_cast<long long>(std::round(x));
    double result = 1.0;
    for (long long i = 2; i <= n; ++i) {
        result *= static_cast<double>(i);
    }
    return result;
}

double sinOf(double x, AngleMode mode) {
    return std::sin(toRadians(x, mode));
}

double cosOf(double x, AngleMode mode) {
    return std::cos(toRadians(x, mode));
}

double tanOf(double x, AngleMode mode) {
    return std::tan(toRadians(x, mode));
}

double asinOf(double x, AngleMode mode) {
    if (x < -1.0 || x > 1.0) {
        throw CalculatorError("Input out of domain for inverse trig function");
    }
    return fromRadians(std::asin(x), mode);
}

double acosOf(double x, AngleMode mode) {
    if (x < -1.0 || x > 1.0) {
        throw CalculatorError("Input out of domain for inverse trig function");
    }
    return fromRadians(std::acos(x), mode);
}

double atanOf(double x, AngleMode mode) {
    return fromRadians(std::atan(x), mode);
}

double log10Of(double x) {
    if (x <= 0.0) {
        throw CalculatorError("Logarithm undefined for non-positive numbers");
    }
    return std::log10(x);
}

double lnOf(double x) {
    if (x <= 0.0) {
        throw CalculatorError("Logarithm undefined for non-positive numbers");
    }
    return std::log(x);
}

double expOf(double x) {
    return std::exp(x);
}

} // namespace engine::mathfn
