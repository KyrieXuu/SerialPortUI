#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <choosewidget.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
namespace Ui {
class loginwidget;
}

class loginwidget : public QWidget
{
    Q_OBJECT

public:
    explicit loginwidget(QWidget *parent = nullptr);
    ~loginwidget();

    void linkdatabase();//连接数据库
    void initUI();//初始化UI

private slots:
    void on_pushButton_clicked();

private:
    const QString User = "admin";
    const QString Code = "123456";

private:
    Ui::loginwidget *ui;
};

#endif // LOGINWIDGET_H
