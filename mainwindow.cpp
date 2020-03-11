#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(sayHello()));
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

void MainWindow::sayHello(){
    ui->label->setText("");
}
