#pragma once

#include <QCheckBox>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include "engine/CalculatorEngine.h"

// Thin Qt Widgets shell around engine::CalculatorEngine. This class only
// translates button presses / key presses into engine calls and renders the
// resulting EvaluationResult back into the display. No arithmetic or
// expression-parsing logic lives here.
class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QWidget* buildButtonGrid();

    void appendText(const QString& text);
    void wrapCurrentExpression(const QString& prefix, const QString& suffix);
    void toggleSign();
    void clearAll();
    void backspace();
    void evaluateAndShowResult();
    void toggleAngleMode();
    void toggleInverseTrig();

    void memoryClear();
    void memoryRecall();
    void memoryAdd();
    void memorySubtract();

    // Called before inserting a digit/constant/wrap-function: if the display
    // currently shows a finished result or an error, start a fresh entry
    // instead of appending onto it.
    void beginFreshEntryIfNeeded();
    // Called before inserting a binary operator: if the display shows a
    // finished result, chain from it; if it shows an error, start fresh.
    void beginChainOrFreshEntry();

    // Formats a value the way evaluateAndShowResult()/memoryRecall() need it:
    // fixed precision, with the '.' that QString::number always produces
    // replaced by the current locale's decimal separator.
    QString formatNumber(double value) const;

    engine::CalculatorEngine m_engine;
    QString m_decimalPoint;
    double m_lastResultValue = 0.0;

    QLineEdit* m_display = nullptr;
    QLabel* m_statusLabel = nullptr;
    QPushButton* m_angleModeButton = nullptr;
    QCheckBox* m_inverseCheckBox = nullptr;

    QPushButton* m_sinButton = nullptr;
    QPushButton* m_cosButton = nullptr;
    QPushButton* m_tanButton = nullptr;

    enum class DisplayState { Editing, Result, Error };
    DisplayState m_state = DisplayState::Editing;
};
