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
    initSerial1();
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
void MainWindow::initSerial1()
{
    // 获取可用串口列表
    QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();

    // 查找第一个可用串口
    if (serialPorts.size() >= 1) {
        portnameStr1 = serialPorts[0].portName();
        //使用串口
        serialIo1 = new QSerialPort(portnameStr1, this);
        //连接信号槽
        // 连接信号和槽函数，并传递上下文对象
        connect(serialIo1, &QSerialPort::readyRead, this, [this]() {
            recvData(serialIo1);
        });

        //点击串口[连接]/[断开]按钮
        connect(ui->btnOpen1, &QPushButton::clicked,this, [this]() mutable {
            if (ui->btnOpen1->text() == "连接") {
                openSerial1();
            } else {
                closeSerial1();
            }
        });
        //设置输入框属性，限制输入内容为IP地址和数字
        ui->ipLineEdit->setInputMask("000.000.000.000;_");
        ui->portLineEdit->setValidator(new QIntValidator(0,65535,this));
        //设置初始值
        ui->ipLineEdit->setText("222.175.204.246");
        ui->portLineEdit->setText("13306");

        //点击【保存设置】按钮连接mqtt
        connect(ui->linkButton,&QPushButton::clicked,this,&MainWindow::connectbroker);

        //修改串口名
        ui->portButton1->setText(portnameStr1);
        if(serialPorts.size() == 1){
            //只有一个串口则下面的不用显示了
            ui->Com2Frame->setVisible(false);
            ui->Com3Frame->setVisible(false);
            ui->linkButton->move(1400,1700);
            qDebug()<<"当前可用1串口"<<portnameStr1;
        }else{
            initSerial2();
        }
    } else {//若没有可用串口则返回
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
}

void MainWindow::initSerial2()
{
    // 获取可用串口列表
    QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();

    // 查找第二个可用串口
    if (serialPorts.size() >= 2) {
        portnameStr2 = serialPorts[1].portName();
        //使用串口
        serialIo2 = new QSerialPort(portnameStr2, this);
        //连接信号槽
        // 连接信号和槽函数，并传递上下文对象
        connect(serialIo2, &QSerialPort::readyRead, this, [this]() {
            recvData(serialIo2);
        });

        //点击串口[连接]/[断开]按钮
        connect(ui->btnOpen2, &QPushButton::clicked, this,[this]() mutable {
            if (ui->btnOpen2->text() == "连接") {
                openSerial2();
            } else {
                closeSerial2();
            }
        });
        //修改串口名
        ui->portButton2->setText(portnameStr2);
        if(serialPorts.size() == 2){
            //只有2个串口下面的不用显示
            ui->Com3Frame->setVisible(false);
            ui->linkButton->move(1400,2300);
            qDebug()<<"当前可用2串口"<<portnameStr2;
        }else{
            initSerial3();
        }
    }

    //波特率
    QStringList baudrateList;
    baudrateList<<"1200"<<"2400"<<"4800"<<"9600"<<"19200"<<"38400"<<"57600"<<"115200";
    ui->boxBaudRate1->addItems(baudrateList);//添加下拉列表选项
    ui->boxBaudRate1->setEditable(true);//串口波特率可编辑
    ui->boxBaudRate1->setCurrentText("9600");//界面中初始值
    ui->boxBaudRate1->setView(new QListView(this));//该设置是配合qss的，不然item行高设置没效果
    //数据位
    QStringList databitList;
    databitList<<"5"<<"6"<<"7"<<"8";
    ui->boxDataBits1->addItems(databitList);
    ui->boxDataBits1->setCurrentText("8");
    ui->boxDataBits1->setView(new QListView(this));
    //校验位
    QStringList parityList;
    parityList<<"No"<<"Even偶"<<"Odd奇"<<"Space"<<"Mark";
    ui->boxParity1->addItems(parityList);
    ui->boxParity1->setCurrentText("No");
    ui->boxParity1->setView(new QListView(this));
    //停止位
    QStringList stopbitList;
    stopbitList<<"1"<<"1.5"<<"2";
    ui->boxStopBits1->addItems(stopbitList);
    ui->boxStopBits1->setCurrentText("1");
    ui->boxStopBits1->setView(new QListView(this));
}
void MainWindow::initSerial3(){
    // 获取可用串口列表
    QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();

    // 查找第二个可用串口
    if (serialPorts.size() >= 3) {
        portnameStr3 = serialPorts[2].portName();
        //使用串口
        serialIo3 = new QSerialPort(portnameStr3, this);
        //连接信号槽
        // 连接信号和槽函数，并传递上下文对象
        connect(serialIo3, &QSerialPort::readyRead, this, [this]() {
            recvData(serialIo3);
        });

        //点击串口[连接]/[断开]按钮
        connect(ui->btnOpen3, &QPushButton::clicked, this,[this]() mutable {
            if (ui->btnOpen3->text() == "连接") {
                openSerial3();
            } else {
                closeSerial3();
            }
        });
        //修改串口名
        ui->portButton3->setText(portnameStr3);
        qDebug()<<"当前可用2串口"<<portnameStr3;
    }

    //波特率
    QStringList baudrateList;
    baudrateList<<"1200"<<"2400"<<"4800"<<"9600"<<"19200"<<"38400"<<"57600"<<"115200";
    ui->boxBaudRate2->addItems(baudrateList);//添加下拉列表选项
    ui->boxBaudRate2->setEditable(true);//串口波特率可编辑
    ui->boxBaudRate2->setCurrentText("9600");//界面中初始值
    ui->boxBaudRate2->setView(new QListView(this));//该设置是配合qss的，不然item行高设置没效果
    //数据位
    QStringList databitList;
    databitList<<"5"<<"6"<<"7"<<"8";
    ui->boxDataBits2->addItems(databitList);
    ui->boxDataBits2->setCurrentText("8");
    ui->boxDataBits2->setView(new QListView(this));
    //校验位
    QStringList parityList;
    parityList<<"No"<<"Even偶"<<"Odd奇"<<"Space"<<"Mark";
    ui->boxParity2->addItems(parityList);
    ui->boxParity2->setCurrentText("No");
    ui->boxParity2->setView(new QListView(this));
    //停止位
    QStringList stopbitList;
    stopbitList<<"1"<<"1.5"<<"2";
    ui->boxStopBits2->addItems(stopbitList);
    ui->boxStopBits2->setCurrentText("1");
    ui->boxStopBits2->setView(new QListView(this));
}

void MainWindow::openSerial1(){
    qDebug()<<"串口1打开";
    if(!portnameStr1.isEmpty()){
        QSerialPortInfo info(portnameStr1);
        if(info.isNull()){
            qDebug()<<"当前串口繁忙,可能已被占用,请确认后再连接"<<portnameStr1;
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
        //串口配置设置
//        serialIo->setPortName(portnameStr);
        serialIo1->setBaudRate(baudrate);
        serialIo1->setDataBits(databit);
        serialIo1->setParity(parity);
        serialIo1->setStopBits(stopbit);
        if(serialIo1->open(QIODevice::ReadWrite)){
            qDebug()<<"串口已打开,读写模式";
            setSerial1Enable(false);//改变ui状态
        }else{
            qDebug()<<"串口打开异常"<<portnameStr1<<serialIo1->errorString();
                                                                   serialIo1->clearError();
            setSerial1Enable(true);
        }
    }else{
        qDebug()<<"未找到可用串口,请确认串口连接正常后点击刷新";
    }
}

void MainWindow::openSerial2(){
    qDebug()<<"串口2打开";
    if(!portnameStr2.isEmpty()){
        QSerialPortInfo info(portnameStr2);
        if(info.isNull()){
            qDebug()<<"当前串口繁忙,可能已被占用,请确认后再连接"<<portnameStr2;
            return;
        }
        //
        qint32 baudrate=ui->boxBaudRate1->currentText().toInt();
        QSerialPort::DataBits databit;
        switch (ui->boxDataBits1->currentIndex()) {
        case 0:databit=QSerialPort::Data5; break;
        case 1:databit=QSerialPort::Data6; break;
        case 2:databit=QSerialPort::Data7; break;
        case 3:databit=QSerialPort::Data8; break;
        default:databit=QSerialPort::Data8; break;
        }
        QSerialPort::Parity parity;
        switch (ui->boxParity1->currentIndex()) {
        case 0:parity=QSerialPort::NoParity; break;
        case 1:parity=QSerialPort::EvenParity; break;
        case 2:parity=QSerialPort::OddParity; break;
        case 3:parity=QSerialPort::SpaceParity; break;
        case 4:parity=QSerialPort::MarkParity; break;
        default:parity=QSerialPort::NoParity; break;
        }
        QSerialPort::StopBits stopbit;
        switch (ui->boxStopBits1->currentIndex()) {
        case 0:stopbit=QSerialPort::OneStop; break;
        case 1:stopbit=QSerialPort::OneAndHalfStop; break;
        case 2:stopbit=QSerialPort::TwoStop; break;
        default:stopbit=QSerialPort::OneStop; break;
        }
        //串口配置设置
        //        serialIo->setPortName(portnameStr);
        serialIo2->setBaudRate(baudrate);
        serialIo2->setDataBits(databit);
        serialIo2->setParity(parity);
        serialIo2->setStopBits(stopbit);
        if(serialIo2->open(QIODevice::ReadWrite)){
            qDebug()<<"串口已打开,读写模式";
            setSerial2Enable(false);//改变ui状态
        }else{
            qDebug()<<"串口打开异常"<<portnameStr2<<serialIo2->errorString();
                                                                    serialIo2->clearError();
            setSerial2Enable(true);
        }
    }else{
        qDebug()<<"未找到可用串口,请确认串口连接正常后点击刷新";
    }
}

void MainWindow::openSerial3(){
    qDebug()<<"串口3打开";
    if(!portnameStr3.isEmpty()){
        QSerialPortInfo info(portnameStr3);
        if(info.isNull()){
            qDebug()<<"当前串口繁忙,可能已被占用,请确认后再连接"<<portnameStr3;
            return;
        }
        //
        qint32 baudrate=ui->boxBaudRate2->currentText().toInt();
        QSerialPort::DataBits databit;
        switch (ui->boxDataBits2->currentIndex()) {
        case 0:databit=QSerialPort::Data5; break;
        case 1:databit=QSerialPort::Data6; break;
        case 2:databit=QSerialPort::Data7; break;
        case 3:databit=QSerialPort::Data8; break;
        default:databit=QSerialPort::Data8; break;
        }
        QSerialPort::Parity parity;
        switch (ui->boxParity2->currentIndex()) {
        case 0:parity=QSerialPort::NoParity; break;
        case 1:parity=QSerialPort::EvenParity; break;
        case 2:parity=QSerialPort::OddParity; break;
        case 3:parity=QSerialPort::SpaceParity; break;
        case 4:parity=QSerialPort::MarkParity; break;
        default:parity=QSerialPort::NoParity; break;
        }
        QSerialPort::StopBits stopbit;
        switch (ui->boxStopBits2->currentIndex()) {
        case 0:stopbit=QSerialPort::OneStop; break;
        case 1:stopbit=QSerialPort::OneAndHalfStop; break;
        case 2:stopbit=QSerialPort::TwoStop; break;
        default:stopbit=QSerialPort::OneStop; break;
        }
        //串口配置设置
        //        serialIo->setPortName(portnameStr);
        serialIo3->setBaudRate(baudrate);
        serialIo3->setDataBits(databit);
        serialIo3->setParity(parity);
        serialIo3->setStopBits(stopbit);
        if(serialIo3->open(QIODevice::ReadWrite)){
            qDebug()<<"串口已打开,读写模式";
            setSerial3Enable(false);//改变ui状态
        }else{
            qDebug()<<"串口打开异常"<<portnameStr3<<serialIo3->errorString();
                                                                    serialIo3->clearError();
            setSerial3Enable(true);
        }
    }else{
        qDebug()<<"未找到可用串口,请确认串口连接正常后点击刷新";
    }
}

void MainWindow::setSerial1Enable(bool enabled)
{
    //打开成功就false不能再修改配置，关闭状态true可以进行设置
    ui->btnOpen1->setText(enabled?QString("连接"):QString("断开"));
    //可以把btn和配置分在两个widget里，这样直接设置widget的enable就没这么麻烦了
    ui->boxBaudRate->setEnabled(enabled);
    ui->boxDataBits->setEnabled(enabled);
    ui->boxParity->setEnabled(enabled);
    ui->boxStopBits->setEnabled(enabled);
//    ui->boxFlowControl->setEnabled(enabled);
}

void MainWindow::setSerial2Enable(bool enabled)
{
    //打开成功就false不能再修改配置，关闭状态true可以进行设置
    ui->btnOpen2->setText(enabled?QString("连接"):QString("断开"));
    //可以把btn和配置分在两个widget里，这样直接设置widget的enable就没这么麻烦了
    ui->boxBaudRate1->setEnabled(enabled);
    ui->boxDataBits1->setEnabled(enabled);
    ui->boxParity1->setEnabled(enabled);
    ui->boxStopBits1->setEnabled(enabled);
}

void MainWindow::setSerial3Enable(bool enabled)
{
    //打开成功就false不能再修改配置，关闭状态true可以进行设置
    ui->btnOpen3->setText(enabled?QString("连接"):QString("断开"));
    //可以把btn和配置分在两个widget里，这样直接设置widget的enable就没这么麻烦了
    ui->boxBaudRate2->setEnabled(enabled);
    ui->boxDataBits2->setEnabled(enabled);
    ui->boxParity2->setEnabled(enabled);
    ui->boxStopBits2->setEnabled(enabled);
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
    bool sign = choose->getable();
    if (serialIo->bytesAvailable()) {
        //串口收到的数据可能不是连续的，需要的话应该把数据缓存下来再进行协议解析，类似tcp数据处理
        const QByteArray recv_data=serialIo->readAll();
        recv_buffer.append(recv_data);
        //接收发送要一致，如果是处理字节数据，可以把QByteArray当数组一样取下标，或者用data()方法转为char*形式
        if( sign == true ){ //若启动则开始处理字节数据
            processCompletePacket(&recv_buffer,serialIo);
        }else{//若没启动则清空缓存区
            recv_buffer.clear();
        }
//        ui->textRecv->append(QString::fromUtf8(recv_data));
//        qDebug()<<"已接收1："<<QString::fromUtf8(recv_data);
//        qDebug()<<"已接收2："<<QString::fromUtf8(recv_buffer);
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

void MainWindow::closeSerial1()
{
    serialIo1->clear();
    serialIo1->close();
    qDebug()<<"串口已关闭"<<portnameStr1;
    setSerial1Enable(true);
}

void MainWindow::closeSerial2()
{
    serialIo2->clear();
    serialIo2->close();
    qDebug()<<"串口已关闭"<<portnameStr2;
    setSerial2Enable(true);
}

void MainWindow::closeSerial3()
{
    serialIo1->clear();
    serialIo1->close();
    qDebug()<<"串口已关闭";
    setSerial3Enable(true);
}

void MainWindow::processCompletePacket(QByteArray* buffer,QSerialPort* serialIo){
//    qDebug()<<"接收到"<<buffer;
//    qDebug()<<"处理接收到："<<buffer->toHex(' ');
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
        qDebug()<<"已接收："<<resultdata;

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


