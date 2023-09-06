#include <QApplication>
#include <QScreen>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    //屏幕居中
    mainWindow.move((QGuiApplication::primaryScreen()->size().width() - mainWindow.width()) / 2, (QGuiApplication::primaryScreen()->size().height() - mainWindow.height()) / 2);
    return QApplication::exec();

}


