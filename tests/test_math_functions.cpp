#include <QtTest/QtTest>

#include "engine/CalculatorError.h"
#include "engine/MathFunctions.h"

using engine::AngleMode;
namespace mathfn = engine::mathfn;

class TestMathFunctions : public QObject {
    Q_OBJECT

private slots:
    void squareRoot_ofPositive();
    void squareRoot_ofNegative_throws();
    void reciprocal_ofNonZero();
    void reciprocal_ofZero_throws();
    void power_basic();
    void factorial_basic();
    void factorial_ofZero_isOne();
    void factorial_ofNegative_throws();
    void factorial_ofNonInteger_throws();
    void factorial_tooLarge_throws();
    void trig_degrees();
    void trig_radians();
    void inverseTrig_degrees();
    void inverseTrig_outOfDomain_throws();
    void log10_ofPositive();
    void log10_ofNonPositive_throws();
    void ln_ofPositive();
    void ln_ofNonPositive_throws();
    void exp_basic();
};

void TestMathFunctions::squareRoot_ofPositive() {
    QCOMPARE(mathfn::squareRoot(16.0), 4.0);
}

void TestMathFunctions::squareRoot_ofNegative_throws() {
    QVERIFY_THROWS_EXCEPTION(engine::CalculatorError, mathfn::squareRoot(-4.0));
}

void TestMathFunctions::reciprocal_ofNonZero() {
    QCOMPARE(mathfn::reciprocal(4.0), 0.25);
}

void TestMathFunctions::reciprocal_ofZero_throws() {
    QVERIFY_THROWS_EXCEPTION(engine::CalculatorError, mathfn::reciprocal(0.0));
}

void TestMathFunctions::power_basic() {
    QCOMPARE(mathfn::power(2.0, 10.0), 1024.0);
}

void TestMathFunctions::factorial_basic() {
    QCOMPARE(mathfn::factorial(5.0), 120.0);
}

void TestMathFunctions::factorial_ofZero_isOne() {
    QCOMPARE(mathfn::factorial(0.0), 1.0);
}

void TestMathFunctions::factorial_ofNegative_throws() {
    QVERIFY_THROWS_EXCEPTION(engine::CalculatorError, mathfn::factorial(-1.0));
}

void TestMathFunctions::factorial_ofNonInteger_throws() {
    QVERIFY_THROWS_EXCEPTION(engine::CalculatorError, mathfn::factorial(2.5));
}

void TestMathFunctions::factorial_tooLarge_throws() {
    QVERIFY_THROWS_EXCEPTION(engine::CalculatorError, mathfn::factorial(1000.0));
}

void TestMathFunctions::trig_degrees() {
    QVERIFY(qAbs(mathfn::sinOf(90.0, AngleMode::Degrees) - 1.0) < 1e-9);
    QVERIFY(qAbs(mathfn::cosOf(180.0, AngleMode::Degrees) - (-1.0)) < 1e-9);
    QVERIFY(qAbs(mathfn::tanOf(45.0, AngleMode::Degrees) - 1.0) < 1e-9);
}

void TestMathFunctions::trig_radians() {
    const double pi = 3.14159265358979323846;
    QVERIFY(qAbs(mathfn::sinOf(pi / 2.0, AngleMode::Radians) - 1.0) < 1e-9);
    QVERIFY(qAbs(mathfn::cosOf(pi, AngleMode::Radians) - (-1.0)) < 1e-9);
}

void TestMathFunctions::inverseTrig_degrees() {
    QVERIFY(qAbs(mathfn::asinOf(1.0, AngleMode::Degrees) - 90.0) < 1e-9);
    QVERIFY(qAbs(mathfn::acosOf(1.0, AngleMode::Degrees) - 0.0) < 1e-9);
    QVERIFY(qAbs(mathfn::atanOf(1.0, AngleMode::Degrees) - 45.0) < 1e-9);
}

void TestMathFunctions::inverseTrig_outOfDomain_throws() {
    QVERIFY_THROWS_EXCEPTION(engine::CalculatorError, mathfn::asinOf(2.0, AngleMode::Degrees));
    QVERIFY_THROWS_EXCEPTION(engine::CalculatorError, mathfn::acosOf(-2.0, AngleMode::Degrees));
}

void TestMathFunctions::log10_ofPositive() {
    QCOMPARE(mathfn::log10Of(100.0), 2.0);
}

void TestMathFunctions::log10_ofNonPositive_throws() {
    QVERIFY_THROWS_EXCEPTION(engine::CalculatorError, mathfn::log10Of(0.0));
    QVERIFY_THROWS_EXCEPTION(engine::CalculatorError, mathfn::log10Of(-5.0));
}

void TestMathFunctions::ln_ofPositive() {
    QVERIFY(qAbs(mathfn::lnOf(2.71828182845904523536) - 1.0) < 1e-9);
}

void TestMathFunctions::ln_ofNonPositive_throws() {
    QVERIFY_THROWS_EXCEPTION(engine::CalculatorError, mathfn::lnOf(0.0));
}

void TestMathFunctions::exp_basic() {
    QVERIFY(qAbs(mathfn::expOf(1.0) - 2.71828182845904523536) < 1e-9);
}

QTEST_APPLESS_MAIN(TestMathFunctions)
#include "test_math_functions.moc"
