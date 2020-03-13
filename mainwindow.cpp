#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->speed->setMinimum(0);
    ui->speed->setMaximum(255);
    ui->speed->setSingleStep(1);
    ui->speed->setPageStep(1);
    ui->height->setMinimum(0);
    ui->height->setMaximum(30);
    ui->height->setSingleStep(1);
    ui->height->setPageStep(1);

    ui->currentSpeed->setText("0");
    ui->currentHeight->setText("0 cm");

    connect(ui->speed,SIGNAL(valueChanged(int)),this,SLOT(updateSpeed()));
    connect(ui->height,SIGNAL(valueChanged(int)),this,SLOT(updateHeight()));

    connect(ui->up,SIGNAL(clicked(bool)),this,SLOT(moveUp()));
    connect(ui->down,SIGNAL(clicked(bool)),this,SLOT(moveDown()));
    connect(ui->stop,SIGNAL(clicked(bool)),this,SLOT(stop()));
    connect(ui->go,SIGNAL(clicked(bool)),this,SLOT(go()));


    udpSocketSend = new QUdpSocket(this);
    udpSocketSend1 = new QUdpSocket(this);
    bcastA1 = new QHostAddress("10,0,0,11");
    udpSocketSend->connectToHost(*bcastA1,4002);
    bcastA2 = new QHostAddress("10,0,0,12");
    udpSocketSend1->connectToHost(*bcastA2,4003);
}

void MainWindow::updateSpeed(){
    this->speed = ui->speed->value();
    ui->currentSpeed->setText(QString::number(this->speed));
}

void MainWindow::updateHeight(){
    this->height = ui->height->value();
    ui->currentHeight->setText(QString::number(ui->height->value())+" cm");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::moveUp(){
    QString qs("up;");
    qs+=ui->speed->value();
    qs+=";";
    this->sendUdpPacket(qs);
}

void MainWindow::moveDown(){
    QString qs("down;");
    qs+=ui->speed->value();
    qs+=";";
    this->sendUdpPacket(qs);
}

void MainWindow::go(){
    QString qs("go;");
    qs+=ui->speed->value();
    qs+=";";
    qs+=ui->height->value();
    qs+=";";
    this->sendUdpPacket(qs);
}

void MainWindow::stop(){
    QString qs("stop;");
    this->sendUdpPacket(qs);
}

QString MainWindow::sendUdpPacket(QString message){
    QByteArray mess = message.toUtf8();
    udpSocketSend->write(mess);
    udpSocketSend1->write(mess);
    return * new QString("Sent");
}
