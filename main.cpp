#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QtUiTools/QtUiTools>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.setWindowTitle("Table Control");
    return a.exec();
}
