#include <QtTest/QtTest>

#include "engine/CalculatorEngine.h"

using engine::AngleMode;
using engine::CalculatorEngine;
using engine::EvaluationResult;

class TestCalculatorEngine : public QObject {
    Q_OBJECT

private slots:
    void init();

    void basicAddition();
    void basicMultiplication();
    void basicDivision();
    void operatorPrecedence();
    void parentheses();
    void nestedParentheses();
    void decimalCalculations();
    void decimalCalculations_commaSeparator();
    void negativeNumbers();
    void unaryMinusVsSubtraction();
    void divisionByZero_isError();
    void invalidExpression_mismatchedParens();
    void invalidExpression_empty();
    void invalidExpression_trailingGarbage();
    void invalidExpression_unknownIdentifier();
    void squareRoot_ok();
    void squareRoot_ofNegative_isError();
    void powers();
    void negativePowerExponent();
    void factorial();
    void percent();
    void constants();
    void trigDegreeMode();
    void trigRadianMode();
    void inverseTrig();
    void logarithms();
    void reciprocal();
    void memoryOperations();
    void overflow_isError();

private:
    CalculatorEngine m_engineStorage;
    CalculatorEngine* m_engine = &m_engineStorage;
};

void TestCalculatorEngine::init() {
    m_engineStorage = CalculatorEngine();
}

void TestCalculatorEngine::basicAddition() {
    auto result = m_engine->evaluate("77 + 3");
    QVERIFY(result.success);
    QCOMPARE(result.value, 80.0);
}

void TestCalculatorEngine::basicMultiplication() {
    auto result = m_engine->evaluate("100 * 9");
    QVERIFY(result.success);
    QCOMPARE(result.value, 900.0);
}

void TestCalculatorEngine::basicDivision() {
    auto result = m_engine->evaluate("9 / 2");
    QVERIFY(result.success);
    QCOMPARE(result.value, 4.5);
}

void TestCalculatorEngine::operatorPrecedence() {
    auto result = m_engine->evaluate("2 + 3 * 4");
    QVERIFY(result.success);
    QCOMPARE(result.value, 14.0);
}

void TestCalculatorEngine::parentheses() {
    auto result = m_engine->evaluate("(2 + 3) * 4");
    QVERIFY(result.success);
    QCOMPARE(result.value, 20.0);
}

void TestCalculatorEngine::nestedParentheses() {
    auto result = m_engine->evaluate("((2 + 3) * (4 - 1)) / 3");
    QVERIFY(result.success);
    QCOMPARE(result.value, 5.0);
}

void TestCalculatorEngine::decimalCalculations() {
    auto result = m_engine->evaluate("1.5 + 2.25");
    QVERIFY(result.success);
    QCOMPARE(result.value, 3.75);
}

void TestCalculatorEngine::decimalCalculations_commaSeparator() {
    auto result = m_engine->evaluate("1,5 + 2,25");
    QVERIFY(result.success);
    QCOMPARE(result.value, 3.75);
}

void TestCalculatorEngine::negativeNumbers() {
    auto result = m_engine->evaluate("-5 + 3");
    QVERIFY(result.success);
    QCOMPARE(result.value, -2.0);
}

void TestCalculatorEngine::unaryMinusVsSubtraction() {
    auto a = m_engine->evaluate("5 - 3");
    QVERIFY(a.success);
    QCOMPARE(a.value, 2.0);

    auto b = m_engine->evaluate("5 - -3");
    QVERIFY(b.success);
    QCOMPARE(b.value, 8.0);

    auto c = m_engine->evaluate("-2^2");
    QVERIFY(c.success);
    QCOMPARE(c.value, -4.0);
}

void TestCalculatorEngine::divisionByZero_isError() {
    auto result = m_engine->evaluate("5 / 0");
    QVERIFY(!result.success);
    QVERIFY(!result.errorMessage.empty());
}

void TestCalculatorEngine::invalidExpression_mismatchedParens() {
    auto result = m_engine->evaluate("(2 + 3");
    QVERIFY(!result.success);
}

void TestCalculatorEngine::invalidExpression_empty() {
    auto result = m_engine->evaluate("");
    QVERIFY(!result.success);
}

void TestCalculatorEngine::invalidExpression_trailingGarbage() {
    auto result = m_engine->evaluate("2 + 3)");
    QVERIFY(!result.success);
}

void TestCalculatorEngine::invalidExpression_unknownIdentifier() {
    auto result = m_engine->evaluate("2 + foo");
    QVERIFY(!result.success);
}

void TestCalculatorEngine::squareRoot_ok() {
    auto result = m_engine->evaluate("sqrt(16)");
    QVERIFY(result.success);
    QCOMPARE(result.value, 4.0);
}

void TestCalculatorEngine::squareRoot_ofNegative_isError() {
    auto result = m_engine->evaluate("sqrt(-4)");
    QVERIFY(!result.success);
}

void TestCalculatorEngine::powers() {
    auto result = m_engine->evaluate("2^10");
    QVERIFY(result.success);
    QCOMPARE(result.value, 1024.0);
}

void TestCalculatorEngine::negativePowerExponent() {
    auto result = m_engine->evaluate("2^-1");
    QVERIFY(result.success);
    QCOMPARE(result.value, 0.5);
}

void TestCalculatorEngine::factorial() {
    auto result = m_engine->evaluate("5!");
    QVERIFY(result.success);
    QCOMPARE(result.value, 120.0);
}

void TestCalculatorEngine::percent() {
    auto result = m_engine->evaluate("50%");
    QVERIFY(result.success);
    QCOMPARE(result.value, 0.5);
}

void TestCalculatorEngine::constants() {
    auto pi = m_engine->evaluate("pi");
    QVERIFY(pi.success);
    QVERIFY(qAbs(pi.value - 3.14159265358979323846) < 1e-9);

    auto e = m_engine->evaluate("e");
    QVERIFY(e.success);
    QVERIFY(qAbs(e.value - 2.71828182845904523536) < 1e-9);
}

void TestCalculatorEngine::trigDegreeMode() {
    m_engine->setAngleMode(AngleMode::Degrees);
    auto result = m_engine->evaluate("sin(90)");
    QVERIFY(result.success);
    QVERIFY(qAbs(result.value - 1.0) < 1e-9);
}

void TestCalculatorEngine::trigRadianMode() {
    m_engine->setAngleMode(AngleMode::Radians);
    auto result = m_engine->evaluate("sin(pi / 2)");
    QVERIFY(result.success);
    QVERIFY(qAbs(result.value - 1.0) < 1e-9);
}

void TestCalculatorEngine::inverseTrig() {
    m_engine->setAngleMode(AngleMode::Degrees);
    auto result = m_engine->evaluate("asin(1)");
    QVERIFY(result.success);
    QVERIFY(qAbs(result.value - 90.0) < 1e-9);
}

void TestCalculatorEngine::logarithms() {
    auto log = m_engine->evaluate("log10(1000)");
    QVERIFY(log.success);
    QCOMPARE(log.value, 3.0);

    auto ln = m_engine->evaluate("ln(e)");
    QVERIFY(ln.success);
    QVERIFY(qAbs(ln.value - 1.0) < 1e-9);

    auto invalid = m_engine->evaluate("log10(-1)");
    QVERIFY(!invalid.success);
}

void TestCalculatorEngine::reciprocal() {
    auto result = m_engine->evaluate("1/(4)");
    QVERIFY(result.success);
    QCOMPARE(result.value, 0.25);
}

void TestCalculatorEngine::memoryOperations() {
    QCOMPARE(m_engine->memoryRecall(), 0.0);

    m_engine->memoryAdd(10.0);
    QCOMPARE(m_engine->memoryRecall(), 10.0);

    m_engine->memoryAdd(5.0);
    QCOMPARE(m_engine->memoryRecall(), 15.0);

    m_engine->memorySubtract(3.0);
    QCOMPARE(m_engine->memoryRecall(), 12.0);

    m_engine->memoryClear();
    QCOMPARE(m_engine->memoryRecall(), 0.0);
}

void TestCalculatorEngine::overflow_isError() {
    auto result = m_engine->evaluate("10^400");
    QVERIFY(!result.success);
}

QTEST_APPLESS_MAIN(TestCalculatorEngine)
#include "test_calculator_engine.moc"
