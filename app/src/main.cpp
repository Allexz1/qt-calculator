#include <QApplication>
#include <QIcon>

#include "MainWindow.h"

namespace {
QIcon loadAppIcon() {
    QIcon icon;
    for (int size : {16, 32, 48, 64, 128, 256}) {
        icon.addFile(QString(":/icons/calculator-%1.png").arg(size), QSize(size, size));
    }
    return icon;
}
} // namespace

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(loadAppIcon());

    MainWindow window;
    window.setWindowTitle("Calculator");
    window.setFixedSize(370, 382);
    window.show();

    return app.exec();
}
