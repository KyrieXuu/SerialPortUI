#ifndef LOGINDATABASE_H
#define LOGINDATABASE_H


#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>


class logindatabase
{
public:
    logindatabase();

    int linkdatabase();     //连接数据库
};

#endif // LOGINDATABASE_H
