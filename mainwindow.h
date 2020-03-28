#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>


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
    void updateSpeed();
    void updateHeight();
    void moveUp();
    void moveDown();
    void stop();
    void go();
    void changeStatus();
    void changeStatus1();
private:
    Ui::MainWindow *ui;
    int speed;
    int height;
    QString sendUdpPacket(QString);
    QUdpSocket *udpSocketSend,*udpSocketSend1;
    QHostAddress *bcastA1,*bcastA2;

};

#endif // MAINWINDOW_H
