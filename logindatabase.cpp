#include "logindatabase.h"

logindatabase::logindatabase()
{

}

int logindatabase::linkdatabase(){
    //连接数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("数据库名");
    db.setUserName("用户名");
    db.setPassword("密码");
    bool ok = db.open();
    //如果数据库连接成功打开窗口否则退出程序
    if (ok){
        QMessageBox::information(this, "Information", "Database connection succeeded.");
    }
    else {
        QString err = QString("Database connection failed: %1").arg(db.lastError().text());
        QMessageBox::warning(QCoreApplication::instance(), "Warning", err);
        qDebug()<<"error open database because"<<db.lastError().text();
    }
}
