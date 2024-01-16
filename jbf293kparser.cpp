#include "jbf293kparser.h"
#include <iostream>
#include <QtCore>

// 初始化函数
JBF293KParser::JBF293KParser()
{
    // 存储接收到的36bit信息（十六进制）
    data = new int[36];

    // 一类报警代码类型
    classOneAlarm = new int[33]{0x00, 0x69, 0x09, 0xEF, 0x01, 0x0B,
                                0x80, 0x0A, 0x81, 0x82, 0x87, 0x88,
                                0x83, 0x84, 0x90, 0x91, 0x85, 0x86,
                                0x98, 0x97, 0x51, 0x52, 0x53, 0x54,
                                0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,
                                0x8b, 0x8c, 0x8d};

    // 二类报警代码类型
    classTwoAlarm = new int[4]{0xfa, 0x70, 0x71, 0x72};
}


// 作者：刘陈林
// 时间：20230817
// 函数：解析信息函数
// 输入：接收的信息（char* 字符串）
// 输出：解析的信息（string 字符串）
string JBF293KParser::parse(const char * info) {

    char *token;
    char temp[200];
    strcpy(temp, info);
    // 分割字符串
    token = strtok(temp, " ");

    // 获取接收的数据并以int形式存储
    int i = 0;
    while (token != NULL) {
        data[i] = stoi(token, 0, 16);
        token = strtok(NULL, " ");
        i++;
    }


    // 计算报警代码，判断报警类型，并进行解析，回传解析后的信息字符串
    int d1 = (data[1] - 0x30) * 16 + (data[2] - 0x30);

    if (find(classOneAlarm, classOneAlarm+33, d1)-classOneAlarm != 33){             // 一类报警信息

        return parseClassOneAlarmCode().toString();

    }else if (find(classTwoAlarm, classTwoAlarm+4, d1)-classTwoAlarm != 4){         // 二类报警信息

        return parseClassTwoAlarmCode().toString();

    }else if (d1==0xfb || d1==0xfc){                                                // 防火门与电气火灾报警

        return parseClassFireDoorAndElectricalFireAlarmCode().toString();

    }else{                                                                          // 未查询到相应报警信息

        return "Invalid data! Code Type Not Found!";

    }
}


// 作者：刘陈林
// 时间：20230817
// 函数：解析一类报警信息函数
// 输入：无
// 输出：一类报警信息结构体（ClassOneAlarmInfo）
ClassOneAlarmInfo JBF293KParser::parseClassOneAlarmCode() {

    ClassOneAlarmInfo classOneAlarmInfo;

    classOneAlarmInfo.ifValid = true;

    //判断开始与结束代码是否正确
    if (data[0] != 0x82) {
        classOneAlarmInfo.ifValid = false;
        classOneAlarmInfo.errorInfo = "first code error "+to_string(data[0]);
        return classOneAlarmInfo;
    }

    if (data[25] != 0x83) {
        classOneAlarmInfo.ifValid = false;
        classOneAlarmInfo.errorInfo = "last code error "+to_string(data[25]);
        return classOneAlarmInfo;
    }


    //转化D1-D12为十进制数据
    int d1 = (data[1] - 0x30) * 16 + (data[2] - 0x30);
    int d2 = (data[3] - 0x30) * 16 + (data[4] - 0x30);
    int d3 = (data[5] - 0x30) * 16 + (data[6] - 0x30);
    int d4 = (data[7] - 0x30) * 16 + (data[8] - 0x30);
    int d5 = (data[9] - 0x30) * 16 + (data[10] - 0x30);
    int d6 = (data[11] - 0x30) * 16 + (data[12] - 0x30);
    int d7 = (data[13] - 0x30) * 16 + (data[14] - 0x30);
    int d8 = (data[15] - 0x30) * 16 + (data[16] - 0x30);
    int d9 = (data[17] - 0x30) * 16 + (data[18] - 0x30);
    int d10 = (data[19] - 0x30) * 16 + (data[20] - 0x30);
    int d11 = (data[21] - 0x30) * 16 + (data[22] - 0x30);


    //检验累加和是否正确
    int d12 = (data[23] - 0x30) * 16 + (data[24] - 0x30);
    int tempsum = (d1+d2+d3+d4+d5+d6+d7+d8+d9+d10+d11)%256;
    if (d12 != tempsum){
        classOneAlarmInfo.ifValid = false;
        classOneAlarmInfo.errorInfo = "sum code error: " + to_string(d12) + " vs " + to_string(tempsum);
        return classOneAlarmInfo;
    }

    //判断报警命令代码
    if (d1 == 0x09){                                                                // 特殊情况，控制器正常的应答
        classOneAlarmInfo.codeInfo = "代码信息：" + transClassOneAlarmCodeInfo(d1)
                                     + " 版本"+to_string(d2)+"."+to_string(d3)+";";
    }else{
        classOneAlarmInfo.codeInfo = "代码信息：" + transClassOneAlarmCodeInfo(d1)+";";
    }

    if (classOneAlarmInfo.codeInfo == "Invalid data!"){                             //  未查询到此类代码，不过这段代码可以删掉
        classOneAlarmInfo.ifValid = false;
        classOneAlarmInfo.errorInfo = "code info error";
        return classOneAlarmInfo;
    }


    //录入信息
    classOneAlarmInfo.machineNum = "控制器号：" + to_string(d2)+";";
    classOneAlarmInfo.circuitNum = "回路号：" + to_string(d3)+";";
    classOneAlarmInfo.partNum = transClassOneAlarmPartNum(d1, d4)+";";
    classOneAlarmInfo.partType = "部件类型：" + to_string(d5)+";";
    classOneAlarmInfo.year = to_string(d6)+"年;";
    classOneAlarmInfo.month = to_string(d7)+"月;";
    classOneAlarmInfo.day = to_string(d8)+"日;";
    classOneAlarmInfo.hour = to_string(d9)+"时;";
    classOneAlarmInfo.miniute = to_string(d10)+"分;";
    classOneAlarmInfo.second = to_string(d11)+"秒;";

    return classOneAlarmInfo;
}


// 作者：刘陈林
// 时间：20230817
// 函数：解析二类报警信息函数
// 输入：无
// 输出：二类报警信息结构体（ClassTwoAlarmInfo）
ClassTwoAlarmInfo JBF293KParser::parseClassTwoAlarmCode() {

    ClassTwoAlarmInfo classTwoAlarmInfo;

    classTwoAlarmInfo.ifValid = true;

    //判断开始与结束代码是否正确
    if (data[0] != 0x82) {
        classTwoAlarmInfo.ifValid = false;
        classTwoAlarmInfo.errorInfo = "first code error "+to_string(data[0]);
        return classTwoAlarmInfo;
    }

    if (data[25] != 0x83) {
        classTwoAlarmInfo.ifValid = false;
        classTwoAlarmInfo.errorInfo = "last code error "+to_string(data[25]);
        return classTwoAlarmInfo;
    }


    //转化D1-D12为十进制数据
    int d1 = (data[1] - 0x30) * 16 + (data[2] - 0x30);
    int d2 = (data[3] - 0x30) * 16 + (data[4] - 0x30);
    int d3 = (data[5] - 0x30) * 16 + (data[6] - 0x30);
    int d4 = (data[7] - 0x30) * 16 + (data[8] - 0x30);
    int d5 = (data[9] - 0x30) * 16 + (data[10] - 0x30);
    int d6 = (data[11] - 0x30) * 16 + (data[12] - 0x30);
    int d7 = (data[13] - 0x30) * 16 + (data[14] - 0x30);
    int d8 = (data[15] - 0x30) * 16 + (data[16] - 0x30);
    int d9 = (data[17] - 0x30) * 16 + (data[18] - 0x30);
    int d10 = (data[19] - 0x30) * 16 + (data[20] - 0x30);
    int d11 = (data[21] - 0x30) * 16 + (data[22] - 0x30);


    //检验累加和是否正确
    int d12 = (data[23] - 0x30) * 16 + (data[24] - 0x30);
    int tempsum = (d1+d2+d3+d4+d5+d6+d7+d8+d9+d10+d11)%256;
    if (d12 != tempsum){
        classTwoAlarmInfo.ifValid = false;
        classTwoAlarmInfo.errorInfo = "sum code error: " + to_string(d12) + " vs " + to_string(tempsum);
        return classTwoAlarmInfo;
    }

    //判断报警命令代码
    classTwoAlarmInfo.codeInfo = "代码信息：" + transClassTwoAlarmCodeInfo(d1, d3)+";";
    if (classTwoAlarmInfo.codeInfo == "Invalid data!"){
        classTwoAlarmInfo.ifValid = false;
        classTwoAlarmInfo.errorInfo = "code info error";
        return classTwoAlarmInfo;
    }


    //录入信息
    classTwoAlarmInfo.machineNum = "机器号：" + to_string(d2)+";";
    classTwoAlarmInfo.partNum = transClassTwoAlarmPartNum(d1, d4)+";";
    classTwoAlarmInfo.year = to_string(d6)+"年;";
    classTwoAlarmInfo.month = to_string(d7)+"月;";
    classTwoAlarmInfo.day = to_string(d8)+"日;";
    classTwoAlarmInfo.hour = to_string(d9)+"时;";
    classTwoAlarmInfo.miniute = to_string(d10)+"分;";
    classTwoAlarmInfo.second = to_string(d11)+"秒;";

    return classTwoAlarmInfo;
}


// 作者：刘陈林
// 时间：20230817
// 函数：解析防火门与电气火灾报警信息函数
// 输入：无
// 输出：防火门与电气火灾报警信息结构体（ClassFireDoorAndElectricalFireAlarmInfo）
ClassFireDoorAndElectricalFireAlarmInfo JBF293KParser::parseClassFireDoorAndElectricalFireAlarmCode() {

    ClassFireDoorAndElectricalFireAlarmInfo classFireDoorAndElectricalFireAlarmInfo;

    classFireDoorAndElectricalFireAlarmInfo.ifValid = true;


    // 判断开始与结束代码是否正确
    if (data[0] != 0x82) {
        classFireDoorAndElectricalFireAlarmInfo.ifValid = false;
        classFireDoorAndElectricalFireAlarmInfo.errorInfo = "first code error "+to_string(data[0]);
        return classFireDoorAndElectricalFireAlarmInfo;
    }


    // 转化D1-D12为十进制数据
    int d1 = (data[1] - 0x30) * 16 + (data[2] - 0x30);
    int d2 = (data[3] - 0x30) * 16 + (data[4] - 0x30);
    int d3 = (data[5] - 0x30) * 16 + (data[6] - 0x30);
    int d4 = (data[7] - 0x30) * 16 + (data[8] - 0x30);
    int d5 = (data[9] - 0x30) * 16 + (data[10] - 0x30);
    int d6 = (data[11] - 0x30) * 16 + (data[12] - 0x30);
    int d7 = (data[13] - 0x30) * 16 + (data[14] - 0x30);
    int d8 = (data[15] - 0x30) * 16 + (data[16] - 0x30);
    int d9 = (data[17] - 0x30) * 16 + (data[18] - 0x30);
    int d10 = (data[19] - 0x30) * 16 + (data[20] - 0x30);
    int d11 = (data[21] - 0x30) * 16 + (data[22] - 0x30);



    // 检验累加和是否正确
    int d12 = (data[23] - 0x30) * 16 + (data[24] - 0x30);
    int tempsum = (d1+d2+d3+d4+d5+d6+d7+d8+d9+d10+d11)%256;
    if (d12 != tempsum){
        classFireDoorAndElectricalFireAlarmInfo.ifValid = false;
        classFireDoorAndElectricalFireAlarmInfo.errorInfo = "sum code error: " + to_string(d12) + " vs " + to_string(tempsum);
        return classFireDoorAndElectricalFireAlarmInfo;
    }


    // 录入共有信息
    classFireDoorAndElectricalFireAlarmInfo.machineNum = "机器号：" + to_string(d2)+";";
    classFireDoorAndElectricalFireAlarmInfo.circuitNum = "回路号：" + to_string(d3)+";";
    classFireDoorAndElectricalFireAlarmInfo.partNum = "部位地址：" + to_string(d4)+";";
    classFireDoorAndElectricalFireAlarmInfo.year = to_string(d6)+"年;";
    classFireDoorAndElectricalFireAlarmInfo.month = to_string(d7)+"月;";
    classFireDoorAndElectricalFireAlarmInfo.day = to_string(d8)+"日;";
    classFireDoorAndElectricalFireAlarmInfo.hour = to_string(d9)+"时;";
    classFireDoorAndElectricalFireAlarmInfo.miniute = to_string(d10)+"分;";
    classFireDoorAndElectricalFireAlarmInfo.second = to_string(d11)+"秒;";


    // 判断是防火门或者是普通电气火灾的报警
    if (d1 == 0xfb || (d1==0xfc && d5!=0xff)){

        classFireDoorAndElectricalFireAlarmInfo.ifSpecialElectricalFire = false;

        // 此时data[25]是最后一位，检查是否正确
        if (data[25] != 0x83) {
            classFireDoorAndElectricalFireAlarmInfo.ifValid = false;
            classFireDoorAndElectricalFireAlarmInfo.errorInfo = "last code error "+to_string(data[25]);
            return classFireDoorAndElectricalFireAlarmInfo;
        }

        //判断防火门部件类型代码
        if(d1 == 0xfb){
            classFireDoorAndElectricalFireAlarmInfo.codeInfo = "代码信息：防火门;";
            classFireDoorAndElectricalFireAlarmInfo.partType = "部件类型：" + transClassFireDoorAlarmPartType()+";";
        }else{
            classFireDoorAndElectricalFireAlarmInfo.codeInfo = "代码信息：电气火灾;";
            classFireDoorAndElectricalFireAlarmInfo.partType = "部件类型：" + transClassElectricalFireAlarmPartType()+";";
        }

        if (classFireDoorAndElectricalFireAlarmInfo.partType == "部件类型：Invalid data!;"){
            classFireDoorAndElectricalFireAlarmInfo.ifValid = false;
            classFireDoorAndElectricalFireAlarmInfo.errorInfo = "code info error";
            return classFireDoorAndElectricalFireAlarmInfo;
        }

    }
    // 电气火灾的特殊情况
    else{

        // 此时data[35]是最后一位，检查是否正确
        if (data[35] != 0x83) {
            classFireDoorAndElectricalFireAlarmInfo.ifValid = false;
            classFireDoorAndElectricalFireAlarmInfo.errorInfo = "last code error "+to_string(data[35]);
            return classFireDoorAndElectricalFireAlarmInfo;
        }

        // 计算额外需要的数据
        int d13 = (data[25] - 0x30) * 16 + (data[26] - 0x30);
        int d14 = (data[27] - 0x30) * 16 + (data[28] - 0x30);
//        int d15 = (data[29] - 0x30) * 16 + (data[30] - 0x30);
        int d16 = (data[31] - 0x30) * 16 + (data[32] - 0x30);

        // 添加CRC8校验代码

//        int dataArray[16];
//        dataArray[0]=d1;
//        dataArray[1]=d2;
//        dataArray[2]=d3;
//        dataArray[3]=d4;
//        dataArray[4]=d5;
//        dataArray[5]=d6;
//        dataArray[6]=d7;
//        dataArray[7]=d8;
//        dataArray[8]=d9;
//        dataArray[9]=d10;
//        dataArray[10]=d11;
//        dataArray[11]=d12;
//        dataArray[12]=d13;
//        dataArray[13]=d14;
//        dataArray[14]=d15;
//        dataArray[15]=d16;


//        char *result = reinterpret_cast<char*>(dataArray);          //将int数组指针转换为指向char类型的指针
//        const QByteArray byteArray(result, sizeof(dataArray));        //将result填充到字节数组
//        quint8 crc = 0;                                         //使用8个比特位（即1字节）来表示一个无符号整数
//        quint8 polynomial = 0x31;                               // CRC8=X^8+X^5+X^4+X^0

//        foreach (char byte, byteArray) {
//            crc ^= byte;                                        //进行异或操作
//            qDebug("An XOR operation was performed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

//            for (int bit = 0; bit < 8; bit++) {
//                if (crc & 0x80) {                               //判断 crc 的最高位是否为1
//                    crc = (crc << 1) ^ polynomial;              //左移后的 crc 和多项式 polynomial 进行按位异或运算
//                    qDebug("An XOR operation with polynomial");
//                } else {
//                    crc <<= 1;
//                    qDebug("Moved one bit to the left");
//                }
//            }
//        }
//        qDebug("output data:");
//        qDebug() << static_cast<int>(crc);
        //QByteArray crc8=QByteArray::fromRawData(reinterpret_cast<const char*>(&crc), 1);//将crc变量的值转为QByteArray对象


        // 录入代码信息与部件类型
        classFireDoorAndElectricalFireAlarmInfo.codeInfo = "代码信息：电气火灾;";
        classFireDoorAndElectricalFireAlarmInfo.partType = "部件类型：" + transClassSpecialElectricalFireAlarmPartType()+";";
        if (classFireDoorAndElectricalFireAlarmInfo.partType == "部件类型：Invalid data!;"){
            classFireDoorAndElectricalFireAlarmInfo.ifValid = false;
            classFireDoorAndElectricalFireAlarmInfo.errorInfo = "code info error";
            return classFireDoorAndElectricalFireAlarmInfo;
        }


        //录入特殊信息
        classFireDoorAndElectricalFireAlarmInfo.ifSpecialElectricalFire = true;
        classFireDoorAndElectricalFireAlarmInfo.sensorChannelNum = "传感器通道：" + to_string(d14)+";";
        classFireDoorAndElectricalFireAlarmInfo.alarmValue = transClassSpecialElectricalFireAlarmAlarmValue(d13, d16)+";";
        if (classFireDoorAndElectricalFireAlarmInfo.alarmValue == "Invalid data!"){
            classFireDoorAndElectricalFireAlarmInfo.ifValid = false;
            classFireDoorAndElectricalFireAlarmInfo.errorInfo = "part type error";
            return classFireDoorAndElectricalFireAlarmInfo;
        }
    }

    return classFireDoorAndElectricalFireAlarmInfo;
}




// 作者：刘陈林
// 时间：20230817
// 函数：解析一类报警信息报警代码函数
// 输入：数据1（d1）（int）
// 输出：一类报警信息报警代码（string 字符串）
string JBF293KParser::transClassOneAlarmCodeInfo(int d1){
    switch (d1) {
    case 0x00:
        return "控制器心跳";
    case 0x69:
        return "控制器心跳";
    case 0x09:
        return "控制器正常";
    case 0xEF:
        return "控制器故障";
    case 0x01:
        return "控制器复位";
    case 0x0B:
        return "控制器消音";
    case 0x80:
        return "火警";
    case 0x0A:
        return "火警";
    case 0x81:
        return "故障";
    case 0x82:
        return "故障恢复";
    case 0x87:
        return "回路故障";
    case 0x88:
        return "回路故障恢复";
    case 0x83:
        return "自动启动";
    case 0x84:
        return "自动停止";
    case 0x90:
        return "手动启动";
    case 0x91:
        return "手动停止";
    case 0x85:
        return "设备回答";
    case 0x86:
        return "回答撤销";
    case 0x98:
        return "部件隔离";
    case 0x97:
        return "部件隔离撤销";
    case 0x51:
        return "多线手动启动";
    case 0x52:
        return "多线手动停止";
    case 0x53:
        return "多线回答";
    case 0x54:
        return "多线停止回答";
    case 0x55:
        return "多线自动启动";
    case 0x56:
        return "多线自动停止";
    case 0x57:
        return "多线自动启动延时";
    case 0x58:
        return "多线应答缺失";
    case 0x59:
        return "多线线路故障";
    case 0x5A:
        return "多线线路故障恢复";
    case 0x8b:
        return "模拟报警";
    case 0x8c:
        return "监管报警";
    case 0x8d:
        return "监管报警撤销";
    default:
        return "Invalid data!";
    }
}


// 作者：刘陈林
// 时间：20230817
// 函数：解析一类报警信息部位号函数
// 输入：数据1、数据4（d1, d4）（int, int）
// 输出：一类报警信息部位号（string 字符串）
string JBF293KParser::transClassOneAlarmPartNum(int d1, int d4){
    if (d1>=0x51 && d1<=0x5A){
        return "盘号："+to_string((data[7]-0x30)/8+1)+"，专线号："+to_string(data[8] - 0x30);
    }else{
        return "部位号：" + to_string(d4);
    }
}




// 作者：许凯
// 时间：20230817
// 函数：解析二类报警信息报警代码函数
// 输入：数据1、数据3（d1, d3）（int, int）
// 输出：二类报警信息报警代码（string 字符串）
string JBF293KParser::transClassTwoAlarmCodeInfo(int d1,int d3){
    switch (d1) {
    case 0xfa:
        switch (d3) {
        case 0x01:
            return "气体线路故障";
        case 0x02:
            return "气体线路故障恢复";
        case 0x03:
            return "气体喷洒应答";
        case 0x04:
            return "气体喷洒应答撤销";
        case 0x05:
            return "声光启动";
        case 0x06:
            return "声光停止";
        case 0x07:
            return "相关设备动作";
        case 0x08:
            return "相关设备动作停止";
        case 0x09:
            return "防火区启动";
        case 0x0a:
            return "防火区停止";
        case 0x0b:
            return "喷洒启动";
        case 0x0c:
            return "延时启动";
        case 0x0e:
            return "远程停止动作撤销";
        default:
            return "Invalid data!";
        }
    case 0x70:
        if ( d3 == 0x80 ){
            return "板故障";
        }
        else if ( d3 == 0x90 ){
            return "板故障恢复";
        }
        else
            return "Invalid data!";
    case 0x71:
        if ( d3 == 0x80 ){
            return "主电故障";
        }
        else if ( d3 == 0x90 ){
            return "主电故障恢复";
        }
        else
            return "Invalid data!";
    case 0x72:
        if ( d3 == 0x80 ){
            return "备电故障";
        }
        else if ( d3 == 0x90 ){
            return "备电故障恢复";
        }
        else
            return "Invalid data!";
    default:
        return "Invalid data!";

    }
}


// 作者：刘陈林
// 时间：20230817
// 函数：解析二类报警信息部位号函数
// 输入：数据1、数据4（d1, d4）（int, int）
// 输出：二类报警信息部位号（string 字符串）
string JBF293KParser::transClassTwoAlarmPartNum(int d1, int d4){
    if (d1 == 0xfa){
        return "盘号："+to_string((data[7]-0x30)/4+1)+"，区号："+to_string(data[8] - 0x30);
    }else if (d1 == 0x70) {
        return "板号：" + to_string(d4);
    }else{
        return "部位：" + to_string(d4);
    }
}



// 作者：许凯
// 时间：20230817
// 函数：解析防火门报警信息部件类型函数
// 输入：无
// 输出：防火门报警信息部件类型（string 字符串）
string JBF293KParser::transClassFireDoorAlarmPartType(){


    //用低位判断防火门类型
    switch (data[10]) {
    case 0x31:
        switch (data[9]) {                         //通过高位判断防火门状态
        case 0x31:
            return "单常开防火门故障";
        case 0x32:
            return "单常开防火门故障撤销";
        case 0x33:
            return "单常开防火门延时关闭";
        case 0x34:
            return "单常开防火门正在关闭（自动）";
        case 0x35:
            return "单常开防火门正在关闭消息撤销(自动)";
        case 0x36:
            return "单常开防火门正在关闭(手动)";
        case 0x37:
            return "单常开防火门正在关闭消息撤销(手动)";
        case 0x38:
            return "单常开防火门成功关闭(由于启动)";
        case 0x39:
            return "单常开防火门成功关闭消息撤销";
        case 0x3a:
            return "单常开防火门屏蔽";
        case 0x3b:
            return "单常开防火门屏蔽撤销";
        default:
            return "Invalid data!";

        }
    case 0x32:
        switch (data[9]) {
        case 0x31:
            return "单常闭防火门故障";
        case 0x32:
            return "单常闭防火门故障撤销";
        case 0x33:
            return "单常闭防火门延时关闭";
        case 0x34:
            return "单常闭防火门正在关闭（自动）";
        case 0x35:
            return "单常闭防火门正在关闭消息撤销(自动)";
        case 0x36:
            return "单常闭防火门正在关闭(手动)";
        case 0x37:
            return "单常闭防火门正在关闭消息撤销(手动)";
        case 0x38:
            return "单常闭防火门成功关闭(由于启动)";
        case 0x39:
            return "单常闭防火门成功关闭消息撤销";
        case 0x3a:
            return "单常闭防火门屏蔽";
        case 0x3b:
            return "单常闭防火门屏蔽撤销";
        default:
            return "Invalid data!";

        }
    case 0x33:
        switch (data[9]) {
        case 0x31:
            return "双常开防火门故障";
        case 0x32:
            return "双常开防火门故障撤销";
        case 0x33:
            return "双常开防火门延时关闭";
        case 0x34:
            return "双常开防火门正在关闭（自动）";
        case 0x35:
            return "双常开防火门正在关闭消息撤销(自动)";
        case 0x36:
            return "双常开防火门正在关闭(手动)";
        case 0x37:
            return "双常开防火门正在关闭消息撤销(手动)";
        case 0x38:
            return "双常开防火门成功关闭(由于启动)";
        case 0x39:
            return "双常开防火门成功关闭消息撤销";
        case 0x3a:
            return "双常开防火门屏蔽";
        case 0x3b:
            return "双常开防火门屏蔽撤销";
        default:
            return "Invalid data!";

        }
    case 0x34:
        switch (data[9]) {
        case 0x31:
            return "双常闭防火门故障";
        case 0x32:
            return "双常闭防火门故障撤销";
        case 0x33:
            return "双常闭防火门延时关闭";
        case 0x34:
            return "双常闭防火门正在关闭（自动）";
        case 0x35:
            return "双常闭防火门正在关闭消息撤销(自动)";
        case 0x36:
            return "双常闭防火门正在关闭(手动)";
        case 0x37:
            return "双常闭防火门正在关闭消息撤销(手动)";
        case 0x38:
            return "双常闭防火门成功关闭(由于启动)";
        case 0x39:
            return "双常闭防火门成功关闭消息撤销";
        case 0x3a:
            return "双常闭防火门屏蔽";
        case 0x3b:
            return "双常闭防火门屏蔽撤销";
        default:
            return "Invalid data!";

        }
    default:
        return "Invalid data!";

    }
}


// 作者：许凯
// 时间：20230817
// 函数：解析电气火灾报警信息部件类型函数
// 输入：无
// 输出：电气火灾报警信息部件类型（string 字符串）
string JBF293KParser::transClassElectricalFireAlarmPartType(){


    //通过低4位判断电气火灾探测器类型
    switch (data[10]) {
    case 0x31:
        switch (data[9]) {         //通过高4位判断电气火灾探测器状态
        case 0x31:
            return "全部探测器报警";
        case 0x32:
            return "全部探测器故障";
        case 0x33:
            return "全部探测器故障撤销";
        case 0x34:
            return "全部探测器脱扣动作(自动)";
        case 0x35:
            return "全部探测器脱扣停止(自动)";
        case 0x36:
            return "全部探测器脱扣动作(手动)";
        case 0x37:
            return "全部探测器脱扣停止(手动)";
        case 0x38:
            return "全部探测器屏蔽";
        case 0x39:
            return "全部探测器屏蔽撤销";
        default:
            return "Invalid data!";
        }
    case 0x32:
        switch (data[9]) {
        case 0x31:
            return "剩余电流探测器报警";
        case 0x32:
            return "剩余电流探测器故障";
        case 0x33:
            return "剩余电流探测器故障撤销";
        case 0x34:
            return "剩余电流探测器脱扣动作(自动)";
        case 0x35:
            return "剩余电流探测器脱扣停止(自动)";
        case 0x36:
            return "剩余电流探测器脱扣动作(手动)";
        case 0x37:
            return "剩余电流探测器脱扣停止(手动)";
        case 0x38:
            return "剩余电流探测器屏蔽";
        case 0x39:
            return "剩余电流探测器屏蔽撤销";
        default:
            return "Invalid data!";
        }
    case 0x33:
        switch (data[9]) {
        case 0x31:
            return "温度探测器报警";
        case 0x32:
            return "温度探测器故障";
        case 0x33:
            return "温度探测器故障撤销";
        case 0x34:
            return "温度探测器脱扣动作(自动)";
        case 0x35:
            return "温度探测器脱扣停止(自动)";
        case 0x36:
            return "温度探测器脱扣动作(手动)";
        case 0x37:
            return "温度探测器脱扣停止(手动)";
        case 0x38:
            return "温度探测器屏蔽";
        case 0x39:
            return "温度探测器屏蔽撤销";
        default:
            return "Invalid data!";
        }
    case 0x34:
        switch (data[9]) {
        case 0x31:
            return "故障电弧探测器报警";
        case 0x32:
            return "故障电弧探测器故障";
        case 0x33:
            return "故障电弧探测器故障撤销";
        case 0x34:
            return "故障电弧探测器脱扣动作(自动)";
        case 0x35:
            return "故障电弧探测器脱扣停止(自动)";
        case 0x36:
            return "故障电弧探测器脱扣动作(手动)";
        case 0x37:
            return "故障电弧探测器脱扣停止(手动)";
        case 0x38:
            return "故障电弧探测器屏蔽";
        case 0x39:
            return "故障电弧探测器屏蔽撤销";
        default:
            return "Invalid data!";
        }
    case 0x35:
        switch (data[9]) {
        case 0x31:
            return "过电流探测器报警";
        case 0x32:
            return "过电流探测器故障";
        case 0x33:
            return "过电流探测器故障撤销";
        case 0x34:
            return "过电流探测器脱扣动作(自动)";
        case 0x35:
            return "过电流探测器脱扣停止(自动)";
        case 0x36:
            return "过电流探测器脱扣动作(手动)";
        case 0x37:
            return "过电流探测器脱扣停止(手动)";
        case 0x38:
            return "过电流探测器屏蔽";
        case 0x39:
            return "过电流探测器屏蔽撤销";
        default:
            return "Invalid data!";
        }
    case 0x36:
        switch (data[9]) {
        case 0x31:
            return "脱扣继电器报警";
        case 0x32:
            return "脱扣继电器故障";
        case 0x33:
            return "脱扣继电器故障撤销";
        case 0x34:
            return "脱扣继电器脱扣动作(自动)";
        case 0x35:
            return "脱扣继电器脱扣停止(自动)";
        case 0x36:
            return "脱扣继电器脱扣动作(手动)";
        case 0x37:
            return "脱扣继电器脱扣停止(手动)";
        case 0x38:
            return "脱扣继电器屏蔽";
        case 0x39:
            return "脱扣继电器屏蔽撤销";
        default:
            return "Invalid data!";
        }
    case 0x37:
        switch (data[9]) {
        case 0x31:
            return "预留报警";
        case 0x32:
            return "预留故障";
        case 0x33:
            return "预留故障撤销";
        case 0x34:
            return "预留脱扣动作(自动)";
        case 0x35:
            return "预留脱扣停止(自动)";
        case 0x36:
            return "预留脱扣动作(手动)";
        case 0x37:
            return "预留脱扣停止(手动)";
        case 0x38:
            return "预留屏蔽";
        case 0x39:
            return "预留屏蔽撤销";
        default:
            return "Invalid data!";
        }
    default:
        return "Invalid data!";
    }
}


// 作者：许凯
// 时间：20230817
// 函数：解析特殊电气火灾报警信息部件类型函数
// 输入：无
// 输出：防火门报警信息部件类型（string 字符串）
string JBF293KParser::transClassSpecialElectricalFireAlarmPartType(){
    switch (data[26]) {
    case 0x32:
        return "剩余电流探测器报警";
    case 0x33:
        return "温度探测器报警";
    case 0x34:
        return "故障电弧探测器报警";
    case 0x35:
        return "过电流探测器报警";
    default:
        return "Invalid data!";

    }
}


// 作者：许凯
// 时间：20230817
// 函数：解析特殊电气火灾报警值函数
// 输入：数据13、数据16（d13, d16）（int, int）
// 输出：特殊电气火灾报警值（string 字符串）
string JBF293KParser::transClassSpecialElectricalFireAlarmAlarmValue(int d13,int d16){

    int term;
    term = ( (data[31]-0x30)*4096 + (data[32]-0x30)*256 + (data[29]-0x30)*16 + data[30]-0x30 )/10;
    switch (d13) {
    case 0x12:
        return "报警值：" + to_string(term) +"mA";
    case 0x13:
        if(d16>=0x80)
            return "报警值：" + to_string(-term) +"度";
        else
            return "报警值：" + to_string(term) +"度";
    case 0x15:
        return "报警值：" + to_string(term/10) +"A";
    case 0x14:
        return "There is no such product";
    default:
        return "Invalid data!";

    }
}
