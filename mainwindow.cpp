#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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


    ui->c_height_1->setAlignment(Qt::AlignCenter);
    ui->c_height_2->setAlignment(Qt::AlignCenter);
    ui->c_height_3->setAlignment(Qt::AlignCenter);
    ui->c_current_1->setAlignment(Qt::AlignCenter);
    ui->c_current_2->setAlignment(Qt::AlignCenter);
    ui->c_current_3->setAlignment(Qt::AlignCenter);
    ui->st_act_1->setProperty("status","disconnected");
//    ui->st_act_1->style()->unpolish(ui->st_act_1);
//    ui->st_act_1->style()->polish(ui->st_act_1);
    ui->st_act_1->update();


    connect(ui->speed,SIGNAL(valueChanged(int)),this,SLOT(updateSpeed()));
    connect(ui->height,SIGNAL(valueChanged(int)),this,SLOT(updateHeight()));

    connect(ui->up,SIGNAL(clicked(bool)),this,SLOT(moveUp()));
    connect(ui->down,SIGNAL(clicked(bool)),this,SLOT(moveDown()));
    connect(ui->stop,SIGNAL(clicked(bool)),this,SLOT(stop()));
    connect(ui->go,SIGNAL(clicked(bool)),this,SLOT(go()));
    connect(ui->balance,SIGNAL(clicked(bool)),this,SLOT(balance()));


    QPixmap pixmap("assets/stop-icon.png");
    QIcon ButtonIcon(pixmap);
    ui->stop->setIcon(ButtonIcon);
    udpSocketSend = new QUdpSocket(this);
    udpSocketSend1 = new QUdpSocket(this);
    udpSocketSend2 = new QUdpSocket(this);
    udpSocketRecieve = new QUdpSocket(this);

    if (udpSocketRecieve->bind(4005))
     {
        ui->status->setText(ui->status->text() + "Bind Ok");
        connect(udpSocketRecieve, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
     }
     else{
        ui->status->setText(ui->status->text() + "bind not ok");
    }
    connect(udpSocketRecieve, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
           this, SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    bcastA1 = new QHostAddress(QHostAddress::Broadcast);
    this->sendUdpPacket("ch;");
    //processPendingDatagrams();
    connect(udpSocketSend,SIGNAL(connected()),this,SLOT(changeStatus()));
}

void MainWindow::updateSpeed(){
    this->speed = ui->speed->value();
    ui->currentSpeed->setText(QString::number(this->speed));
}

void MainWindow::updateHeight(){
    this->height = ui->height->value();
    ui->currentHeight->setText(QString::number(ui->height->value())+" cm");
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
}

void MainWindow::stop(){
    QString qs("st;");
    this->sendUdpPacket(qs);
}

void MainWindow::balance(){
    QString qs("bl;");
    this->sendUdpPacket(qs);
}


QString MainWindow::sendUdpPacket(QString message){
    QByteArray mess = message.toUtf8();
//    udpSocketSend->write(mess);
//    udpSocketSend1->write(mess);

    if(ui->act1_enable->isChecked()){
        if(udpSocketSend->writeDatagram(mess,QHostAddress::Broadcast,4001)){
            ui->status->setText(ui->status->text() + "Sent to port 4001\n");
            udpSocketSend->flush();
        }else{
            ui->status->setText(ui->status->text() + "Error port 4001\n");
        }
    }
    if(ui->act2_enable->isChecked()){
        if(udpSocketSend1->writeDatagram(mess,QHostAddress::Broadcast,4002)){
            ui->status->setText(ui->status->text() + "Sent to port 4002\n");
            udpSocketSend1->flush();
        }else{
            ui->status->setText(ui->status->text() + "Error port 4002\n");
        }
    }
    if(ui->act3_enable->isChecked()){
        if(udpSocketSend2->writeDatagram(mess,QHostAddress::Broadcast,4003)){
            ui->status->setText(ui->status->text() + "Sent to port 4003\n");
            udpSocketSend2->flush();
        }else{
            ui->status->setText(ui->status->text() + "Error port 4003\n");
        }
    }



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



void MainWindow::processPendingDatagrams()
 {
//    ui->status->setText(ui->status->text() + "something came in");
    QHostAddress sender;
    u_int16_t port;
    //ui->status->setText(ui->status->text() + sender.toString());
    while (udpSocketRecieve->hasPendingDatagrams())
    {
         QByteArray datagram;
         datagram.resize(udpSocketRecieve->pendingDatagramSize());
         udpSocketRecieve->readDatagram(datagram.data(),datagram.size(),&sender,&port);
         //ui->status->setText("Message :: " + datagram);
         if(datagram == "ack" && port == 4010){
             ui->st_act_1->setText("Connected");
             ui->st_act_1->setStyleSheet("background-color:rgb(0, 0, 0);color: rgb(0, 255, 0);");
         }
         if(datagram == "ack" && port == 4011){
             ui->st_act_2->setText("Connected");
             ui->st_act_2->setStyleSheet("background-color:rgb(0, 0, 0);color: rgb(0, 255, 0);");
         }
         if(datagram == "ack" && port == 4012){
             ui->st_act_3->setText("Connected");
             ui->st_act_3->setStyleSheet("background-color:rgb(0, 0, 0);color: rgb(0, 255, 0);");
         }
         else{
             QString str  = QString(datagram);
             QStringList List;
             List = str.split(";");
             if(List.at(0)=="h"){
                 if(List.at(1).toInt() == 1){
                     ui->c_height_1->setValue(List.at(2).toInt());
                 }
                 if(List.at(1).toInt()==2){
                     ui->c_height_2->setValue(List.at(2).toInt());
                 }
                 if(List.at(1).toInt() ==3){
                     ui->c_height_3->setValue(List.at(2).toInt());
                 }

             }else if(List.at(0) == "RP"){
                 double roll = 0.0,pitch = 0.0,roll11 = 0.0,pitch11 = 0.0;
                 if(List.at(1).toInt() == 4010){
                     roll = List.at(2).toDouble();
                     pitch = List.at(3).toDouble();
                     ui->status->setText(ui->status->text() + "Act 1 Roll:" + QString::number(roll) + " Pitch:" + QString::number(pitch));
                 }
                 if(List.at(1).toInt() == 4011){
                     roll11 = List.at(2).toDouble();
                     pitch11 = List.at(3).toDouble();
                     ui->status->setText(ui->status->text() + "Act 2 Roll:" + QString::number(roll11) + " Pitch:" + QString::number(pitch11));
                 }
             }

         }
         udpSocketRecieve->flush();
    }
}

void MainWindow::onSocketStateChange (QAbstractSocket::SocketState state) {
    ui->status->setText(ui->status->text() + "connecting slot read");
    if ( state == QAbstractSocket::BoundState ) {

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

