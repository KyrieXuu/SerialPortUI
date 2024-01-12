#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainUi(){
    setWindowState(Qt::WindowMaximized);//最大化
    QImage *img_mainicon;//主图标显示在右上角lable中
    img_mainicon =new QImage;//新建一个image对象
    img_mainicon->load(":/new/prefix1/gem-rhombus.png");
    ui->label_24->setScaledContents(true);
    ui->label_24->setPixmap(QPixmap::fromImage(*img_mainicon));
    ui->label_33->setScaledContents(true);
    ui->label_33->setPixmap(QPixmap::fromImage(*img_mainicon));
    ui->label_34->setScaledContents(true);
    ui->label_34->setPixmap(QPixmap::fromImage(*img_mainicon));
}

void MainWindow::on_pushButton_3_clicked()
{
    this->close();
    choosewidget *choose = new choosewidget;
    choose->show();
}

