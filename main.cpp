#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("番茄钟");

    MainWindow w;
    w.show();

    return app.exec();
}
