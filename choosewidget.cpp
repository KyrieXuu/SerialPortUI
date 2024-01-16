#include "choosewidget.h"
#include <mainwindow.h>

choosewidget::choosewidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::choosewidget),
    mWindow(nullptr) // 初始化为 null 指针
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);//最大化
}

choosewidget::~choosewidget()
{
    delete ui;
}

void choosewidget::setStart(bool able){

    ui->startButton->setText(able?QString("暂停"):QString("启动"));
    //项目启动后不能再设置
    ui->pushButton_2->setEnabled(!able);
    able = !able;
    if(able){
        qDebug()<<"项目启动";
    }else{
        qDebug()<<"项目关闭";
    }
}

void choosewidget::on_pushButton_2_clicked()
{
    this->close();
    if (mWindow == nullptr) {
        mWindow = new MainWindow; // 创建 MainWindow 对象
    }
    mWindow->show();
}

void choosewidget::on_startButton_clicked()
{
    if (mWindow == nullptr) {
        mWindow = new MainWindow; // 创建 MainWindow 对象
    }
    mWindow->setStart();
}

