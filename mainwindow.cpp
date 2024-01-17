#include "mainwindow.h"
#include <choosewidget.h>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    choose = new choosewidget(); // 创建 choosewidget 对象
    initMainUi();
    initSerial();
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

void MainWindow::initSerial(){

    // 获取可用串口列表
    QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();

    // 检测可用串口数量
    if (serialPorts.size() >= 3) {
        qDebug()<<"当前可用串口足够";
        // 有至少三个可用串口
        portnameStr1 = serialPorts[0].portName();
        portnameStr2 = serialPorts[1].portName();
        portnameStr3 = serialPorts[2].portName();

        // 使用这三个串口
        QSerialPort *serialIo1 = new QSerialPort(portnameStr1, this);
        QSerialPort *serialIo2 = new QSerialPort(portnameStr2, this);
        QSerialPort *serialIo3 = new QSerialPort(portnameStr3, this);

        // 连接信号槽
        connect(serialIo1, &QSerialPort::readyRead, std::bind(&MainWindow::recvData, this, serialIo1));
        connect(serialIo2, &QSerialPort::readyRead, std::bind(&MainWindow::recvData, this, serialIo2));
        connect(serialIo3, &QSerialPort::readyRead, std::bind(&MainWindow::recvData, this, serialIo3));
        //点击串口[连接]/[断开]按钮
        connect(ui->btnOpen1,&QPushButton::clicked,this,[this,serialIo1]()mutable{
            if(ui->btnOpen1->text()=="连接"){
                openSerial(portnameStr1,serialIo1);
            }else{
                closeSerial(serialIo1);
            }
        });
        //点击串口[连接]/[断开]按钮
        connect(ui->btnOpen2,&QPushButton::clicked,this,[this,serialIo2]()mutable{
            if(ui->btnOpen2->text()=="连接"){
                openSerial(portnameStr2,serialIo2);
            }else{
                closeSerial(serialIo2);
            }
        });
        //修改串口名
        ui->portButton1->setText(portnameStr1);
        ui->portButton2->setText(portnameStr2);
    } else if(serialPorts.size() >= 1){
        qDebug()<<"当前可用串口COM1";
        // 有串口，但不到3个
        portnameStr1 = serialPorts[0].portName();
        //使用串口
        QSerialPort *serialIo1 = new QSerialPort(portnameStr1, this);
        //连接信号槽
        connect(serialIo1, &QSerialPort::readyRead, this,std::bind(&MainWindow::recvData, this, serialIo1));

        //点击串口[连接]/[断开]按钮
        connect(ui->btnOpen1,&QPushButton::clicked,this,[this,serialIo1]()mutable{
            if(ui->btnOpen1->text()=="连接"){
                openSerial(portnameStr1,serialIo1);
            }else{
                closeSerial(serialIo1);
            }
        });
        //修改串口名
        ui->portButton1->setText(portnameStr1);
        ui->Com2Frame->setVisible(false);
    }else{//若没有可用串口则返回
        qDebug()<<"当前无可用串口";
        this->close();
        choose->show();
    }

    //波特率
    QStringList baudrateList;
    baudrateList<<"1200"<<"2400"<<"4800"<<"9600"<<"19200"<<"38400"<<"57600"<<"115200";
    ui->boxBaudRate->addItems(baudrateList);//添加下拉列表选项
    ui->boxBaudRate->setEditable(true);//串口波特率可编辑
    ui->boxBaudRate->setCurrentText("9600");//界面中初始值
    ui->boxBaudRate->setView(new QListView(this));//该设置是配合qss的，不然item行高设置没效果
    //数据位
    QStringList databitList;
    databitList<<"5"<<"6"<<"7"<<"8";
    ui->boxDataBits->addItems(databitList);
    ui->boxDataBits->setCurrentText("8");
    ui->boxDataBits->setView(new QListView(this));
    //校验位
    QStringList parityList;
    parityList<<"No"<<"Even偶"<<"Odd奇"<<"Space"<<"Mark";
    ui->boxParity->addItems(parityList);
    ui->boxParity->setCurrentText("No");
    ui->boxParity->setView(new QListView(this));
    //停止位
    QStringList stopbitList;
    stopbitList<<"1"<<"1.5"<<"2";
    ui->boxStopBits->addItems(stopbitList);
    ui->boxStopBits->setCurrentText("1");
    ui->boxStopBits->setView(new QListView(this));
    //流控制
    //    QStringList flowctrlList;
    //    flowctrlList<<"No"<<"Hardware"<<"Software";
    //    ui->boxFlowControl->addItems(flowctrlList);
    //    ui->boxFlowControl->setCurrentText("No");
    //    ui->boxFlowControl->setView(new QListView(this));

    //IP地址和端口号
//    QPushButton *LinkPushButton = new QPushButton(this);
//   LinkPushButton->setText("连接");


    //设置输入框属性，限制输入内容为IP地址和数字
    ui->ipLineEdit->setInputMask("000.000.000.000;_");
    ui->portLineEdit->setValidator(new QIntValidator(0,65535,this));


    //点击[保存设置]按钮
    connect(ui->linkButton,&QPushButton::clicked,this,&MainWindow::connectbroker);
}
void MainWindow::setStart(){
    choose->setStart(able);
}

void MainWindow::openSerial(QString portnameStr,QSerialPort* serialIo){

    if(!portnameStr.isEmpty()){
        QSerialPortInfo info(portnameStr);
        if(info.isNull()){
            qDebug()<<"当前串口繁忙,可能已被占用,请确认后再连接"<<portnameStr;
            return;
        }
        //
        qint32 baudrate=ui->boxBaudRate->currentText().toInt();
        QSerialPort::DataBits databit;
        switch (ui->boxDataBits->currentIndex()) {
        case 0:databit=QSerialPort::Data5; break;
        case 1:databit=QSerialPort::Data6; break;
        case 2:databit=QSerialPort::Data7; break;
        case 3:databit=QSerialPort::Data8; break;
        default:databit=QSerialPort::Data8; break;
        }
        QSerialPort::Parity parity;
        switch (ui->boxParity->currentIndex()) {
        case 0:parity=QSerialPort::NoParity; break;
        case 1:parity=QSerialPort::EvenParity; break;
        case 2:parity=QSerialPort::OddParity; break;
        case 3:parity=QSerialPort::SpaceParity; break;
        case 4:parity=QSerialPort::MarkParity; break;
        default:parity=QSerialPort::NoParity; break;
        }
        QSerialPort::StopBits stopbit;
        switch (ui->boxStopBits->currentIndex()) {
        case 0:stopbit=QSerialPort::OneStop; break;
        case 1:stopbit=QSerialPort::OneAndHalfStop; break;
        case 2:stopbit=QSerialPort::TwoStop; break;
        default:stopbit=QSerialPort::OneStop; break;
        }
//        QSerialPort::FlowControl flowcontrol;
//        switch (ui->boxFlowControl->currentIndex()) {
//        case 0:flowcontrol=QSerialPort::NoFlowControl; break;
//        case 1:flowcontrol=QSerialPort::HardwareControl; break;
//        case 2:flowcontrol=QSerialPort::SoftwareControl; break;
//        default:flowcontrol=QSerialPort::NoFlowControl; break;
//        }
        //串口配置设置
//        serialIo->setPortName(portnameStr);
        serialIo->setBaudRate(baudrate);
        serialIo->setDataBits(databit);
        serialIo->setParity(parity);
        serialIo->setStopBits(stopbit);
//        serialIo->setFlowControl(flowcontrol);//这个我一般没用
        if(serialIo->open(QIODevice::ReadWrite)){
            qDebug()<<"串口已打开,读写模式";
            setSerialEnable(false);//改变ui状态
        }else{
            qDebug()<<"串口打开异常"<<portnameStr<<serialIo->errorString();
                                                                   serialIo->clearError();
            setSerialEnable(true);
        }
    }else{
        qDebug()<<"未找到可用串口,请确认串口连接正常后点击刷新";
    }
}


void MainWindow::setSerialEnable(bool enabled)
{
    //打开成功就false不能再修改配置，关闭状态true可以进行设置
//    ui->btnRefresh->setEnabled(enabled);
    ui->btnOpen1->setText(enabled?QString("连接"):QString("断开"));
    //可以把btn和配置分在两个widget里，这样直接设置widget的enable就没这么麻烦了
//    ui->boxPortName->setEnabled(enabled);
    ui->boxBaudRate->setEnabled(enabled);
    ui->boxDataBits->setEnabled(enabled);
    ui->boxParity->setEnabled(enabled);
    ui->boxStopBits->setEnabled(enabled);
//    ui->boxFlowControl->setEnabled(enabled);
}

void MainWindow::sendACK(QSerialPort* serialIo){
    //    const QByteArray send_data=ui->textSend->toPlainText().toUtf8();
    //    if(send_data.size()<=0)
    //        return;
    QByteArray answer = QByteArray::fromStdString(yiaiparse.answerPacks());
    QByteArray hexData = QByteArray::fromHex(answer);
    qDebug()<<answer<<" "<<hexData.toHex();
    if(serialIo->isOpen()){
        qint64 bytesWritten = serialIo->write(hexData.constData(), hexData.size());
        qDebug() << "已发送字节数：" << bytesWritten;
        //        serialIo->write(answer);
        //        qDebug()<<"已发送："<<QString::fromUtf8(answer);
    }else{
        qDebug()<<"发送失败,串口未打开";
        return;
    }
    //Qt新版本默认值是30 000
    if(!serialIo->waitForBytesWritten(30000)){
        qDebug()<<"命令发送异常"<<serialIo->errorString();
                                                serialIo->clearError();
    }
}

void MainWindow::recvData(QSerialPort* serialIo)
{
    static QByteArray recv_buffer;
    if (serialIo->bytesAvailable()) {
        //串口收到的数据可能不是连续的，需要的话应该把数据缓存下来再进行协议解析，类似tcp数据处理
        const QByteArray recv_data=serialIo->readAll();
        recv_buffer.append(recv_data);
        //接收发送要一致，如果是处理字节数据，可以把QByteArray当数组一样取下标，或者用data()方法转为char*形式
        if( able == true ){ //若启动则开始处理字节数据
            processCompletePacket(&recv_buffer,serialIo);
        }else{//若没启动则清空缓存区
            recv_buffer.clear();
        }
//        ui->textRecv->append(QString::fromUtf8(recv_data));
//        qDebug()<<"已接收1："<<QString::fromUtf8(recv_data);
        qDebug()<<"已接收2："<<QString::fromUtf8(recv_buffer);
    }
}

void MainWindow::connectbroker()
{
    //连接后不能再更改配置
    ui->ipLineEdit->setReadOnly(true);
    ui->portLineEdit->setReadOnly(true);
    mqttIP = MainWindow::ui->ipLineEdit->text();
    mqttPort = MainWindow::ui->portLineEdit->text().toInt();
    qDebug() << mqttIP << "  " << mqttPort;
    mqttclient.connect(mqttIP, mqttPort);
}

void MainWindow::closeSerial(QSerialPort* serialIo)
{

    serialIo->clear();
    serialIo->close();
    qDebug()<<"串口已关闭";
    setSerialEnable(true);
}

void MainWindow::processCompletePacket(QByteArray* buffer,QSerialPort* serialIo){
//    qDebug()<<"接收到"<<buffer;
    qDebug()<<"处理接收到："<<buffer->toHex(' ');
    const QByteArray start_flag = QByteArray::fromHex(sf.toLatin1());  // 青鸟起始标志为 0x82
    const QByteArray end_flag = QByteArray::fromHex(ef.toLatin1());    // 青鸟结束标志为 0x83
    const QByteArray begin_flag = QByteArray::fromHex(bf.toLatin1());  // 依爱起始标志为 0x68
    const QByteArray finish_flag = QByteArray::fromHex(ff.toLatin1());    // 依爱结束标志为 0x16

    // 检查缓冲区中是否存在起始标志和结束标志
    int start_pos = buffer->indexOf(start_flag);
    int end_pos = buffer->indexOf(end_flag);
    int begin_pos = buffer->indexOf(begin_flag);
    QByteArray hexValue = buffer->mid(begin_pos + 1, 1);
//    QByteArray hexValue = buffer[begin_pos+2];
//    char hexValue = buffer->at(begin_pos+1);
//    int length_value = hexValue.toInt(nullptr,16);
    int length_value = hexValue.toHex(' ').toInt(nullptr, 16);
//    int finish_pos = buffer->indexOf(finish_flag);

//    qDebug() << begin_pos << "  " << length_value;

    if (start_pos >= 0 && end_pos >= 0 && end_pos > start_pos) {
        // 提取完整的数据包
        QByteArray complete_packet = buffer->mid(start_pos, end_pos - start_pos + 1);

        // 在这里处理完整的数据包，例如解析、显示等操作
        QString hex_str = complete_packet.toHex(' ');
        string parsed_str = jbf293kparser.parse(hex_str.toUtf8().constData());
        QString resultdata = QString::fromStdString(parsed_str);
//        ui->textRecv->append(resultdata);
//        qDebug()<<"已接收："<<resultdata;

        // 存储心跳与告警信息
        if (resultdata.indexOf("心跳")>0){
            saveDataInTxtFile("D:/Qt/heartbeat.txt", resultdata);
        }else{
            saveDataInTxtFile("D:/Qt/warning.txt", resultdata);
        }

        // MQTT发送
        mqttclient.publishJSONMessage("testtopic", resultdata, 0);

        // 清除已处理的数据包
        buffer->remove(0, end_pos + 1);

    }else if(begin_pos>=0 && length_value>=0 && buffer->mid(begin_pos, 1)==buffer->mid(begin_pos+3, 1) \
               && buffer->mid(begin_pos+length_value+6-1, 1)==finish_flag){
//        qDebug()<<"进入了yiai解析";
        // 提取完整的数据包
        QByteArray complete_packet = buffer->mid(begin_pos, length_value+6);

        // 在这里处理完整的数据包，例如解析、显示等操作
        QString hex_str = complete_packet.toHex(' ');
        string parsed_str = yiaiparse.parse(hex_str.toUtf8().constData());
        QString resultdata = QString::fromStdString(parsed_str);
//        ui->textRecv->append(resultdata);
//        qDebug()<<"已接收："<<resultdata;

        // 存储心跳与告警信息
//        if (resultdata.indexOf("心跳")>0){
//            saveDataInTxtFile("D:/Qt/heartbeat.txt", resultdata);
//        }else{
//            saveDataInTxtFile("D:/Qt/warning.txt", resultdata);
//        }
        //打印系统时间
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString currentTimeString = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        //存储数据
        saveDataInTxtFile("D:/Qt/yiai2.txt", currentTimeString);
        saveDataInTxtFile("D:/Qt/yiai2.txt", buffer->toHex(' '));
        saveDataInTxtFile("D:/Qt/yiai2.txt", resultdata);
        //在工控机上存储数据
//        saveDataInTxtFile("D:/消防项目/解析数据.txt", currentTimeString);
//        saveDataInTxtFile("D:/消防项目/解析数据.txt", buffer->toHex(' '));
//        saveDataInTxtFile("D:/消防项目/解析数据.txt", resultdata);


        //通过串口发送应答包
        sendACK(serialIo);
//        //通过串口发送巡检包
//        sendInspection();

        // MQTT发送
        mqttclient.publishJSONMessage("testtopic", resultdata, 0);

        // 清除已处理的数据包
        buffer->remove(0, begin_pos + length_value+6);
//        qDebug()<<"清除后："<<buffer->toHex(' ');
        // 递归调用，继续处理剩余的数据
        processCompletePacket(buffer,serialIo);
    }else{
        flag++;
//        qDebug()<<"标志："<<flag;
        if(flag >2){        //如果连续两次数据错误则清除缓存
//            qDebug()<<"循环标志："<<flag;
            //若数据错误，则清除缓存
            buffer->clear();
//            qDebug()<<"错误数据清除后："<<buffer->toHex(' ');
            flag = 0;
        }
    }
}


void MainWindow::saveDataInTxtFile(QString filepath, QString data){

    QFile file(filepath);

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream textStream(&file);

        textStream << data << Qt::endl; // 将内容写入文本流

        file.close();
    }

}
void MainWindow::on_linkButton_clicked()
{
    this->close();
    choose->show();
}


