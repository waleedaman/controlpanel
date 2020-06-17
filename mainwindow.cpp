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
    ui->stop->setStyleSheet("background-color: red");

    ui->currentSpeed->setText("0");
    ui->currentHeight->setText("0 cm");


    ui->c_height->setAlignment(Qt::AlignCenter);
    ui->c_height_2->setAlignment(Qt::AlignCenter);
    ui->c_height_3->setAlignment(Qt::AlignCenter);
    ui->c_height_4->setAlignment(Qt::AlignCenter);
    ui->c_height_5->setAlignment(Qt::AlignCenter);
    ui->c_height_6->setAlignment(Qt::AlignCenter);


    connect(ui->speed,SIGNAL(valueChanged(int)),this,SLOT(updateSpeed()));
    connect(ui->height,SIGNAL(valueChanged(int)),this,SLOT(updateHeight()));

    connect(ui->up,SIGNAL(clicked(bool)),this,SLOT(moveUp()));
    connect(ui->down,SIGNAL(clicked(bool)),this,SLOT(moveDown()));
    connect(ui->stop,SIGNAL(clicked(bool)),this,SLOT(stop()));
    connect(ui->go,SIGNAL(clicked(bool)),this,SLOT(go()));
    QPixmap pixmap("assets/stop-icon.png");
    QIcon ButtonIcon(pixmap);
    ui->stop->setIcon(ButtonIcon);
    udpSocketSend = new QUdpSocket(this);
    udpSocketSend1 = new QUdpSocket(this);
    udpSocketSend->bind(4005);
    bcastA1 = new QHostAddress(QHostAddress::Broadcast);
//    udpSocketSend->connectToHost(*bcastA1,4002);
    bcastA2 = new QHostAddress("10,0,0,13");
//    udpSocketSend1->connectToHost(*bcastA2,4003);
    connect(udpSocketSend,SIGNAL(connected()),this,SLOT(changeStatus()));
//    connect(udpSocketSend1,SIGNAL(connected()),this,SLOT(changeStatus1()));

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
    QString qs("mu;");
    qs+=QString::number(ui->speed->value());
    qs+=";";
    this->sendUdpPacket(qs);
}

void MainWindow::moveDown(){
    QString qs("md;");
    qs+=QString::number(ui->speed->value());
    qs+=";";
    this->sendUdpPacket(qs);
}

void MainWindow::go(){
    QString qs("go;");
    qs+=QString::number(ui->speed->value());
    qs+=";";
    qs+=QString::number(ui->height->value());
    qs+=";";
    this->sendUdpPacket(qs);
//    ui->->display(ui->height->value());
//    ui->current_height->setText(QString::number(ui->height->value()));
    ui->c_height->setValue(ui->height->value());
}

void MainWindow::stop(){
    QString qs("st;");
    this->sendUdpPacket(qs);
}

QString MainWindow::sendUdpPacket(QString message){
    QByteArray mess = message.toUtf8();
//    udpSocketSend->write(mess);
//    udpSocketSend1->write(mess);
    if(udpSocketSend->writeDatagram(mess,QHostAddress::Broadcast,4002)){
        ui->status->setText("sent");
    }else{
        ui->status->setText("Error");
    }
    if(udpSocketSend->writeDatagram(mess,QHostAddress::Broadcast,4002)){
        ui->status->setText("sent");
    }else{
        ui->status->setText("Error");
    }
    udpSocketSend1->writeDatagram(mess,*bcastA2,4001);
    udpSocketSend1->writeDatagram(mess,*bcastA2,4002);
    udpSocketSend1->writeDatagram(mess,*bcastA2,4003);
    return * new QString("Sent");
}

void MainWindow::changeStatus(){
    QString str = ui->status->text();
    ui->status->setText(str+"connected to port: 4002");
}

void MainWindow::changeStatus1(){
    QString str = ui->status->text();
    ui->status->setText(str+"connected to port: 4003");
}
