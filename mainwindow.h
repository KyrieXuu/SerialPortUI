#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "jbf293kparser.h"
#include "yiaiparser.h"
#include <QFile>
#include "mqttclient.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QListView>
class choosewidget;
class QSerialPort;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initSerial();  //初始化串口设置
    void initMainUi();  //初始化界面操作

    void openSerial(QString portnameStr,QSerialPort* serialIo);  //槽函数-打开串口
    void closeSerial(QSerialPort* serialIo); //槽函数-关闭串口
    void connectbroker();//连接服务器

    void setStart();//连接后不能修改该串口配置

    void saveDataInTxtFile(QString filepath, QString data);     //将解析的数据保存到文件中


private slots:
    void on_linkButton_clicked();

private:
    void recvData(QSerialPort* serialIo);        //槽函数-接收数据
    void sendACK(QSerialPort* serialIo);//yiai发送应答包
    const QString sf = "82"; //青鸟协议开始标志
    const QString ef = "83"; //青鸟协议结束标志
    const QString bf = "68"; //依爱RS232协议开始标志
    const QString ff = "16"; //依爱RS232协议结束标志
    void setSerialEnable(bool enabled);//连接后不能修改该串口配置
    void processCompletePacket(QByteArray* buffer,QSerialPort* serialIo); //处理不连续数据并解析

    QString mqttIP;  //MQTT连接IP
    int mqttPort;  //MQTT连接端口
    int flag = 0;  //防止出现一组数据多次传输
    bool able = false;//设置是否启动
    QString portnameStr1;//串口名
    QString portnameStr2;
    QString portnameStr3;

private:
    Ui::MainWindow *ui;

//    QSerialPort *serialIo;//串口io，一般可以把串口io扔到线程里去，避免阻塞ui

    choosewidget *choose;//choosewidget对象

    JBF293KParser jbf293kparser; // 青鸟解析类

    YiaiParse yiaiparse;        //依爱解析类

    MQTTClient mqttclient;
};
#endif // MAINWINDOW_H
