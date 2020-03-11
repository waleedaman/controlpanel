#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//    void setSpeed(int);
//    void setHeight(int);
//    int getSpeed();
//    int getHeight();
private slots:
    void sayHello();
    void updateSpeed();
    void updateHeight();
private:
    Ui::MainWindow *ui;
    int speed;
    int height;
};

#endif // MAINWINDOW_H
