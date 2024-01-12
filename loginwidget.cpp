#include "loginwidget.h"
#include "ui_loginwidget.h"

loginwidget::loginwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginwidget)
{
    ui->setupUi(this);
    QTimer::singleShot(2000, [this]() {
        linkdatabase();
    });

}

loginwidget::~loginwidget()
{
    delete ui;
}

void loginwidget::on_pushButton_clicked()
{
    this->close();
    choosewidget *choose = new choosewidget();
    choose->show();
}

void loginwidget::linkdatabase(){
    //连接数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("firefighting");
    db.setUserName("root");
    db.setPassword("1234");
    bool ok = db.open();
    //如果数据库连接成功打开窗口否则退出程序
//    if (ok){
//        QMessageBox::information(this, "Information", "Database connection succeeded.");
//    }
//    else {
//        QMessageBox::information(this, "infor", "open failed");
//        qDebug()<<"error open database because"<<db.lastError().text();
//        this->close();
//    }
}
