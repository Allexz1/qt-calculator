#include "MainWindow.h"

#include <QFont>
#include <QGridLayout>
#include <QLocale>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>

using engine::AngleMode;
using engine::EvaluationResult;

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Calculator");
    m_decimalPoint = QLocale::system().decimalPoint();

    m_display = new QLineEdit(this);
    m_display->setReadOnly(false);
    m_display->setAlignment(Qt::AlignRight);
    QFont displayFont = m_display->font();
    displayFont.setPointSize(displayFont.pointSize() + 8);
    m_display->setFont(displayFont);
    m_display->setMinimumSize(1, 1);
    // A physical calculator has no alphabet key: function names and constants
    // only ever reach the display via the dedicated buttons (which insert
    // text programmatically, bypassing this validator). Direct keyboard/paste
    // input is restricted to digits, operators, parens, and decimal points.
    m_display->setValidator(new QRegularExpressionValidator(
        QRegularExpression("[0-9+\\-*/^!%().,]*"), m_display));

    m_statusLabel = new QLabel("DEG", this);
    m_statusLabel->setAlignment(Qt::AlignLeft);
    m_statusLabel->setMinimumSize(1, 1);

    auto* topRow = new QVBoxLayout();
    topRow->addWidget(m_statusLabel);
    topRow->addWidget(m_display);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topRow);
    mainLayout->addWidget(buildButtonGrid());

    setLayout(mainLayout);
    setMinimumSize(1, 1);

    connect(m_display, &QLineEdit::returnPressed, this, &MainWindow::evaluateAndShowResult);
    m_display->installEventFilter(this);

    m_display->setFocus();
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event) {
    if (watched == m_display && event->type() == QEvent::KeyPress) {
        auto* keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key()) {
            case Qt::Key_Backspace:
                if (m_state != DisplayState::Editing) {
                    clearAll();
                    return true;
                }
                return false;
            case Qt::Key_Plus:
            case Qt::Key_Minus:
            case Qt::Key_Asterisk:
            case Qt::Key_Slash:
            case Qt::Key_AsciiCircum:
                beginChainOrFreshEntry();
                return false;
            default:
                break;
        }
        QString typed = keyEvent->text();
        if (typed.isEmpty() || !typed.at(0).isPrint()) {
            return false;
        }
        bool isUnmodifiedLetter = typed.at(0).isLetter() &&
            !(keyEvent->modifiers() & (Qt::ControlModifier | Qt::MetaModifier));
        if (isUnmodifiedLetter) {
            return true; // no alphabet on this calculator's keyboard
        }
        beginFreshEntryIfNeeded();
        return false;
    }
    return QWidget::eventFilter(watched, event);
}

QWidget* MainWindow::buildButtonGrid() {
    auto* container = new QWidget(this);
    auto* grid = new QGridLayout(container);
    container->setLayout(grid);

    auto addButton = [&](const QString& label, int row, int col, int rowSpan = 1, int colSpan = 1) {
        auto* button = new QPushButton(label);
        button->setMinimumSize(1, 1);
        grid->addWidget(button, row, col, rowSpan, colSpan);
        return button;
    };

    // Row 0: memory + mode controls
    auto* mc = addButton("MC", 0, 0);
    auto* mr = addButton("MR", 0, 1);
    auto* mPlus = addButton("M+", 0, 2);
    auto* mMinus = addButton("M-", 0, 3);
    m_angleModeButton = addButton("DEG", 0, 4);
    m_inverseCheckBox = new QCheckBox("Inv", container);
    m_inverseCheckBox->setMinimumSize(1, 1);
    grid->addWidget(m_inverseCheckBox, 0, 5);

    // Row 1: scientific functions
    auto* sqrtBtn = addButton(QString::fromUtf8("\xE2\x88\x9A"), 1, 0);   // sqrt
    auto* squareBtn = addButton("x²", 1, 1);
    auto* reciprocalBtn = addButton("1/x", 1, 2);
    auto* powerBtn = addButton("x^y", 1, 3);
    auto* factorialBtn = addButton("n!", 1, 4);
    auto* clearBtn = addButton("C", 1, 5);

    // Row 2: trig + logs
    m_sinButton = addButton("sin", 2, 0);
    m_cosButton = addButton("cos", 2, 1);
    m_tanButton = addButton("tan", 2, 2);
    auto* log10Btn = addButton("log", 2, 3);
    auto* lnBtn = addButton("ln", 2, 4);
    auto* backspaceBtn = addButton(QString::fromUtf8("\xE2\x8C\xAB"), 2, 5); // backspace

    // Rows 3-6: digits and basic operators
    auto* b7 = addButton("7", 3, 0);
    auto* b8 = addButton("8", 3, 1);
    auto* b9 = addButton("9", 3, 2);
    auto* lparen = addButton("(", 3, 3);
    auto* rparen = addButton(")", 3, 4);
    auto* divideBtn = addButton(QString::fromUtf8("\xC3\xB7"), 3, 5);

    auto* b4 = addButton("4", 4, 0);
    auto* b5 = addButton("5", 4, 1);
    auto* b6 = addButton("6", 4, 2);
    auto* piBtn = addButton(QString::fromUtf8("\xCF\x80"), 4, 3);
    auto* eBtn = addButton("e", 4, 4);
    auto* multiplyBtn = addButton(QString::fromUtf8("\xC3\x97"), 4, 5);

    auto* b1 = addButton("1", 5, 0);
    auto* b2 = addButton("2", 5, 1);
    auto* b3 = addButton("3", 5, 2);
    auto* percentBtn = addButton("%", 5, 3);
    auto* signBtn = addButton(QString::fromUtf8("\xC2\xB1"), 5, 4);
    auto* minusBtn = addButton(QString::fromUtf8("\xE2\x88\x92"), 5, 5);

    auto* b0 = addButton("0", 6, 0, 1, 2);
    auto* dotBtn = addButton(m_decimalPoint, 6, 2);
    auto* equalsBtn = addButton("=", 6, 3, 1, 2);
    auto* plusBtn = addButton("+", 6, 5);

    for (auto* button : {b0, b1, b2, b3, b4, b5, b6, b7, b8, b9}) {
        connect(button, &QPushButton::clicked, this, [this, button]() {
            beginFreshEntryIfNeeded();
            appendText(button->text());
        });
    }
    connect(dotBtn, &QPushButton::clicked, this, [this]() {
        beginFreshEntryIfNeeded();
        appendText(m_decimalPoint);
    });
    connect(lparen, &QPushButton::clicked, this, [this]() {
        beginFreshEntryIfNeeded();
        appendText("(");
    });
    connect(rparen, &QPushButton::clicked, this, [this]() { appendText(")"); });
    connect(piBtn, &QPushButton::clicked, this, [this]() {
        beginFreshEntryIfNeeded();
        appendText("pi");
    });
    connect(eBtn, &QPushButton::clicked, this, [this]() {
        beginFreshEntryIfNeeded();
        appendText("e");
    });

    connect(divideBtn, &QPushButton::clicked, this, [this]() {
        beginChainOrFreshEntry();
        appendText("/");
    });
    connect(multiplyBtn, &QPushButton::clicked, this, [this]() {
        beginChainOrFreshEntry();
        appendText("*");
    });
    connect(minusBtn, &QPushButton::clicked, this, [this]() {
        beginChainOrFreshEntry();
        appendText("-");
    });
    connect(plusBtn, &QPushButton::clicked, this, [this]() {
        beginChainOrFreshEntry();
        appendText("+");
    });
    connect(powerBtn, &QPushButton::clicked, this, [this]() {
        beginChainOrFreshEntry();
        appendText("^");
    });

    connect(factorialBtn, &QPushButton::clicked, this, [this]() { appendText("!"); });
    connect(percentBtn, &QPushButton::clicked, this, [this]() { appendText("%"); });

    connect(sqrtBtn, &QPushButton::clicked, this, [this]() { wrapCurrentExpression("sqrt(", ")"); });
    connect(squareBtn, &QPushButton::clicked, this, [this]() { wrapCurrentExpression("(", ")^2"); });
    connect(reciprocalBtn, &QPushButton::clicked, this, [this]() { wrapCurrentExpression("1/(", ")"); });
    connect(log10Btn, &QPushButton::clicked, this, [this]() { wrapCurrentExpression("log10(", ")"); });
    connect(lnBtn, &QPushButton::clicked, this, [this]() { wrapCurrentExpression("ln(", ")"); });

    connect(m_sinButton, &QPushButton::clicked, this, [this]() {
        wrapCurrentExpression(m_inverseCheckBox->isChecked() ? "asin(" : "sin(", ")");
    });
    connect(m_cosButton, &QPushButton::clicked, this, [this]() {
        wrapCurrentExpression(m_inverseCheckBox->isChecked() ? "acos(" : "cos(", ")");
    });
    connect(m_tanButton, &QPushButton::clicked, this, [this]() {
        wrapCurrentExpression(m_inverseCheckBox->isChecked() ? "atan(" : "tan(", ")");
    });
    connect(m_inverseCheckBox, &QCheckBox::toggled, this, [this]() { toggleInverseTrig(); });

    connect(signBtn, &QPushButton::clicked, this, [this]() { toggleSign(); });
    connect(clearBtn, &QPushButton::clicked, this, [this]() { clearAll(); });
    connect(backspaceBtn, &QPushButton::clicked, this, [this]() { backspace(); });
    connect(equalsBtn, &QPushButton::clicked, this, [this]() { evaluateAndShowResult(); });

    connect(m_angleModeButton, &QPushButton::clicked, this, [this]() { toggleAngleMode(); });

    connect(mc, &QPushButton::clicked, this, [this]() { memoryClear(); });
    connect(mr, &QPushButton::clicked, this, [this]() { memoryRecall(); });
    connect(mPlus, &QPushButton::clicked, this, [this]() { memoryAdd(); });
    connect(mMinus, &QPushButton::clicked, this, [this]() { memorySubtract(); });

    return container;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            evaluateAndShowResult();
            return;
        case Qt::Key_Escape:
            clearAll();
            return;
        default:
            break;
    }
    QWidget::keyPressEvent(event);
}

void MainWindow::beginFreshEntryIfNeeded() {
    if (m_state != DisplayState::Editing) {
        m_display->clear();
        m_state = DisplayState::Editing;
    }
}

void MainWindow::beginChainOrFreshEntry() {
    if (m_state == DisplayState::Error) {
        m_display->clear();
    }
    m_state = DisplayState::Editing;
}

void MainWindow::appendText(const QString& text) {
    m_display->insert(text);
}

void MainWindow::wrapCurrentExpression(const QString& prefix, const QString& suffix) {
    beginChainOrFreshEntry();
    QString current = m_display->text();
    m_display->setText(prefix + current + suffix);
    m_display->end(false);
}

void MainWindow::toggleSign() {
    beginChainOrFreshEntry();
    QString current = m_display->text();
    if (current.startsWith("-(") && current.endsWith(")")) {
        m_display->setText(current.mid(2, current.size() - 3));
    } else {
        m_display->setText("-(" + current + ")");
    }
    m_display->end(false);
}

void MainWindow::clearAll() {
    m_display->clear();
    m_state = DisplayState::Editing;
}

void MainWindow::backspace() {
    if (m_state != DisplayState::Editing) {
        clearAll();
        return;
    }
    QString current = m_display->text();
    if (!current.isEmpty()) {
        current.chop(1);
        m_display->setText(current);
    }
}

QString MainWindow::formatNumber(double value) const {
    QString text = QString::number(value, 'g', 12);
    if (m_decimalPoint != ".") {
        text.replace('.', m_decimalPoint);
    }
    return text;
}

void MainWindow::evaluateAndShowResult() {
    // The engine's tokenizer already accepts both '.' and ',' as a decimal
    // point, so the expression text can be passed through as typed.
    EvaluationResult result = m_engine.evaluate(m_display->text().toStdString());
    if (result.success) {
        m_lastResultValue = result.value;
        m_display->setText(formatNumber(result.value));
        m_state = DisplayState::Result;
    } else {
        m_display->setText(QString::fromStdString(result.errorMessage));
        m_state = DisplayState::Error;
    }
}

void MainWindow::toggleAngleMode() {
    bool nowDegrees = m_engine.angleMode() != AngleMode::Degrees;
    m_engine.setAngleMode(nowDegrees ? AngleMode::Degrees : AngleMode::Radians);
    QString label = nowDegrees ? "DEG" : "RAD";
    m_angleModeButton->setText(label);
    m_statusLabel->setText(label);
}

void MainWindow::toggleInverseTrig() {
    bool inv = m_inverseCheckBox->isChecked();
    m_sinButton->setText(inv ? "asin" : "sin");
    m_cosButton->setText(inv ? "acos" : "cos");
    m_tanButton->setText(inv ? "atan" : "tan");
}

void MainWindow::memoryClear() {
    m_engine.memoryClear();
}

void MainWindow::memoryRecall() {
    beginFreshEntryIfNeeded();
    m_lastResultValue = m_engine.memoryRecall();
    m_display->setText(formatNumber(m_lastResultValue));
    m_state = DisplayState::Result;
}

void MainWindow::memoryAdd() {
    evaluateAndShowResult();
    if (m_state == DisplayState::Result) {
        m_engine.memoryAdd(m_lastResultValue);
    }
}

void MainWindow::memorySubtract() {
    evaluateAndShowResult();
    if (m_state == DisplayState::Result) {
        m_engine.memorySubtract(m_lastResultValue);
    }
}
