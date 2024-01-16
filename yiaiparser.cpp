#include "yiaiparser.h"

YiaiParse::YiaiParse()
{
    data = new int[256]; // 初始化data数组
    response = new int[15];//初始化response数组
    instruction = new int[15];//初始化instruction数组
}


// 函数：解析信息函数
// 输入：接收的信息（char* 字符串）
// 输出：解析的信息（string 字符串）
string YiaiParse::parse(const char * info) {

    char *token;
    char temp[800];
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

    if (data[10]==0x87)    {                                                  // 一类报警信息

        return parseClassOneAlarmCode().toString();

    }else if (data[10]==0x8a){                                                // 二类报警信息

        return parseClassTwoAlarmCode().toString();

    }else if (data[10]==0x8d){                                                // 三类报警信息

        return parseClassThreeAlarmCode().toString();

    }else if(data[10]==0x8b || data[10]==0x8c){                                //复位信息或巡检返回信息

        return parseClassAdditionalCode().toString();

    }else if(data[10]==0xfa){                                                   //应答包

        return parseClassResponseCode().toString();

    }
    else{                                                                          // 未查询到相应报警信息

        return "Invalid data! Code Type Not Found!";

    }
}


// 函数：解析一类报警信息函数
// 输入：无
// 输出：一类报警信息结构体（ClassOneAlarmInfo）
ClassOneAlarmInfo YiaiParse::parseClassOneAlarmCode() {

        ClassOneAlarmInfo classOneAlarmInfo;

        classOneAlarmInfo.ifValid = true;

        //判断包头、版本号、结束字节是否正确
        if( data[0] != 0x68 || data[3] != 0x68 ){
            classOneAlarmInfo.ifValid = false;
            classOneAlarmInfo.errorInfo = "包头错误： " + to_string(data[0]) + to_string(data[3]);
        }
        if(data[9]!=0x29){
            classOneAlarmInfo.ifValid = false;
            classOneAlarmInfo.errorInfo = "版本号错误： " + to_string(data[9]);
        }
        if(data[data[1]+5]!=0x16){
            classOneAlarmInfo.ifValid = false;
            classOneAlarmInfo.errorInfo = "结束字节错误： " + to_string(data[data[1]+5]);
        }

        //检验累加和是否正确
        int tempsum = 0;
        for(int i=0;i<data[1];i++){
            tempsum += data[i+4];
        }
        tempsum = tempsum % 256; // 只保留低八位

        if (data[data[1]+4] != tempsum){
            classOneAlarmInfo.ifValid = false;
            classOneAlarmInfo.errorInfo = "sum code error: " + to_string(data[data[1]+4]) + " vs " + to_string(tempsum);
            return classOneAlarmInfo;
        }

        //解析报警信息
        int numObjects = data[11];
        int startIndex = 12;
        for(int i=0;i<numObjects;i++){
            startIndex += i*38;
            //判断警报
            classOneAlarmInfo.alarmType += "报警类型：" + transClassOneAlarmCodeInfo(data[startIndex+2])+";";

            //回路与地址
            classOneAlarmInfo.circuitNum += "回路：" + to_string(data[startIndex])+";";
            classOneAlarmInfo.address += "地址：" + to_string(data[startIndex+1])+";";
        }

        //判断警报
//        classOneAlarmInfo.alarmType += "报警类型：" + transClassOneAlarmCodeInfo(data[14])+";";

        //判断是增加还是减少
        if((data[15] & 0x80) == 0x80) {
            classOneAlarmInfo.changeStatus += "增加";
        }else
        {
            classOneAlarmInfo.changeStatus += "减少";
        }

        //将时间和分转回十六进制
        QString d16 = QString::number(data[16], 16);
        QString d17 = QString::number(data[17], 16);

        //存入结构体
        int uNum=data[4]+data[5];
        int mNum=data[6]+data[7];
        classOneAlarmInfo.unitNum = "本机号：" + to_string(uNum)+";";
        classOneAlarmInfo.machineNum = "目标机号：" + to_string(mNum)+";";
        classOneAlarmInfo.objectNum = "对象数目：" + to_string(data[11])+";";
//        classOneAlarmInfo.circuitNum = "回路：" + to_string(data[12])+";";
//        classOneAlarmInfo.address = "地址：" + to_string(data[13])+";";
        classOneAlarmInfo.hour = d16.toStdString() + "时;";
        classOneAlarmInfo.minute = d17.toStdString()+"分;";

        return classOneAlarmInfo;
}

// 函数：解析一类报警信息报警代码函数
// 输入：数据1（d14）（int）
// 输出：一类报警信息报警代码（string 字符串）
string YiaiParse::transClassOneAlarmCodeInfo(int d14){
        switch (d14) {
        case 0x01:
            return "火警";
        case 0x16:
            return "感烟火警";
        case 0x17:
            return "感温火警";
        case 0x02:
            return "盗警";
        case 0x03:
            return "预警";
        case 0x04:
            return "摘头故障";
        case 0x05:
            return "老化故障";
        case 0x07:
            return "短路隔离故障";
        case 0x08:
            return "未编故障";
        case 0x0A:
            return "动作";
        case 0x0B:
            return "隔离";
        case 0x0C:
            return "正在启动（总线模块）";
        case 0x0D:
            return "泄漏";
        case 0x0E:
            return "输入模块火警";
        case 0x0F:
            return "控制模块正在延时";
        case 0x18:
            return "烟传感故障";
        case 0x19:
            return "温传感故障";
        case 0x1A:
            return "地址重复";
        case 0x1B:
            return "24V 断线";
        case 0x2A:
            return "输入输出口越界";
        case 0x2B:
            return "信号线断线";
        case 0x34:
            return "多线启动";
        case 0x35:
            return "多线反馈";
        default:
            return "Invalid data!";
        }
}

// 函数：解析二类报警信息函数
// 输入：无
// 输出：二类报警信息结构体（ClassTwoAlarmInfo）
ClassTwoAlarmInfo YiaiParse::parseClassTwoAlarmCode(){

        ClassTwoAlarmInfo classTwoAlarmInfo;

        classTwoAlarmInfo.ifValid = true;

        //判断包头、版本号、结束字节是否正确
        if( data[0] != 0x68 || data[3] != 0x68 ){
            classTwoAlarmInfo.ifValid = false;
            classTwoAlarmInfo.errorInfo = "包头错误： " + to_string(data[0]) + to_string(data[3]);
        }
        if(data[9]!=0x29){
            classTwoAlarmInfo.ifValid = false;
            classTwoAlarmInfo.errorInfo = "版本号错误： " + to_string(data[9]);
        }
        if(data[data[1]+5]!=0x16){
            classTwoAlarmInfo.ifValid = false;
            classTwoAlarmInfo.errorInfo = "结束字节错误： " + to_string(data[data[1]+5]);
        }

        //检验累加和是否正确
        int tempsum = 0;
        for(int i=0;i<data[1];i++){
            tempsum += data[i+4];
        }
        tempsum = tempsum % 256; // 只保留低八位

        if (data[data[1]+4] != tempsum){
            classTwoAlarmInfo.ifValid = false;
            classTwoAlarmInfo.errorInfo = "sum code error: " + to_string(data[data[1]+4]) + " vs " + to_string(tempsum);
            return classTwoAlarmInfo;
        }

        //解析报警信息
        int numObjects = data[11];
        int startIndex = 12;
        for(int i=0;i<numObjects;i++){
            startIndex += i*38;
            //判断警报
            classTwoAlarmInfo.alarmType += "报警类型：" + transClassTwoAlarmCodeInfo(data[startIndex])+";";

            //机号、回路或板号
            classTwoAlarmInfo.circuitNum += "回路/版号：" + to_string(data[startIndex+2])+";";
            classTwoAlarmInfo.number += "机号：" + to_string(data[startIndex+1])+";";
        }
        //判断警报
//        classTwoAlarmInfo.alarmType = "报警类型：" + transClassTwoAlarmCodeInfo(data[12])+";";

        //将时间和分转回十六进制
        QString d15 = QString::number(data[15], 16);
        QString d16 = QString::number(data[16], 16);

        //存入结构体
        int uNum=data[4]+data[5];
        int mNum=data[6]+data[7];
        classTwoAlarmInfo.unitNum = "本机号：" + to_string(uNum)+";";
        classTwoAlarmInfo.machineNum = "目标机号：" + to_string(mNum)+";";
        classTwoAlarmInfo.objectNum = "对象数目：" + to_string(data[11])+";";
//        classTwoAlarmInfo.circuitNum = "回路/版号：" + to_string(data[14])+";";
//        classTwoAlarmInfo.number = "机号：" + to_string(data[13])+";";
        classTwoAlarmInfo.hour = d15.toStdString() + "时;";
        classTwoAlarmInfo.minute = d16.toStdString()+"分;";
        return classTwoAlarmInfo;
}

// 函数：解析二类报警信息报警代码函数
// 输入：数据1（d12）（int）
// 输出：二类报警信息报警代码（string 字符串）
string YiaiParse::transClassTwoAlarmCodeInfo(int d12){
        switch (d12) {
        case 0x10:
            return "短路故障";
        case 0x11:
            return "断路故障";
        default:
            return "Invalid data!";
        }
}


// 函数：解析三类故障信息函数
// 输入：无
// 输出：三类故障信息结构体（ClassThreeAlarmInfo）
ClassThreeAlarmInfo YiaiParse::parseClassThreeAlarmCode(){

        ClassThreeAlarmInfo classThreeAlarmInfo;

        classThreeAlarmInfo.ifValid = true;

        //判断包头、版本号、结束字节是否正确
        if( data[0] != 0x68 || data[3] != 0x68 ){
            classThreeAlarmInfo.ifValid = false;
            classThreeAlarmInfo.errorInfo = "包头错误： " + to_string(data[0]) + to_string(data[3]);
        }
        if(data[9]!=0x29){
            classThreeAlarmInfo.ifValid = false;
            classThreeAlarmInfo.errorInfo = "版本号错误： " + to_string(data[9]);
        }
        if(data[data[1]+5]!=0x16){
            classThreeAlarmInfo.ifValid = false;
            classThreeAlarmInfo.errorInfo = "结束字节错误： " + to_string(data[data[1]+5]);
        }

        //检验累加和是否正确
        int tempsum = 0;
        for(int i=0;i<data[1];i++){
            tempsum += data[i+4];
        }
        tempsum = tempsum % 256; // 只保留低八位

        if (data[data[1]+4] != tempsum){
            classThreeAlarmInfo.ifValid = false;
            classThreeAlarmInfo.errorInfo = "sum code error: " + to_string(data[data[1]+4]) + " vs " + to_string(tempsum);
            return classThreeAlarmInfo;
        }

        //解析报警信息
        int numObjects = data[11];
        int startIndex = 12;
        for(int i=0;i<numObjects;i++){
            startIndex += i*38;
            //判断警报
            classThreeAlarmInfo.faultType += "报警类型：" + transClassThreeAlarmCodeInfo(data[startIndex])+";";

            //机号、回路或板号
            classThreeAlarmInfo.boardNum += transClassThreeAlarmBoardNum(data[startIndex+2])+";";
            classThreeAlarmInfo.number += "机号：" + to_string(data[startIndex+1])+";";
        }
        //判断警报
//        classThreeAlarmInfo.faultType = "报警类型：" + transClassThreeAlarmCodeInfo(data[12])+";";

        //将时间和分转回十六进制
        QString d15 = QString::number(data[15], 16);
        QString d16 = QString::number(data[16], 16);

        //存入结构体
        int uNum=data[4]+data[5];
        int mNum=data[6]+data[7];
        classThreeAlarmInfo.unitNum = "本机号：" + to_string(uNum)+";";
        classThreeAlarmInfo.machineNum = "目标机号：" + to_string(mNum)+";";
        classThreeAlarmInfo.objectNum = "对象数目：" + to_string(data[11])+";";
//        classThreeAlarmInfo.boardNum = transClassThreeAlarmBoardNum(data[14])+";";
//        classThreeAlarmInfo.number = "机号：" + to_string(data[13])+";";
        classThreeAlarmInfo.hour = d15.toStdString() + "时;";
        classThreeAlarmInfo.minute = d16.toStdString()+"分;";


        return classThreeAlarmInfo;
}

// 函数：解析三类报警信息报警代码函数
// 输入：数据1（d12）（int）
// 输出：三类报警信息报警代码（string 字符串）
string YiaiParse::transClassThreeAlarmCodeInfo(int d12){
        switch (d12) {
        case 0x13:
            return "直接按键输出断路";
        case 0x14:
            return "直接按键输出短路";
        case 0x15:
            return "24V 电源输出短路";
        case 0x1C:
            return "液晶显示盘主板故障";
        case 0x1F:
            return "联动键盘主板故障";
        case 0x20:
            return "百接按键板通讯故障";
        case 0x21:
            return "联动键盘板通讯故障";
        case 0x28:
            return "主电故障";
        case 0x29:
            return "备电故障";
        case 0x33:
            return "区域火警";
        case 0x36:
            return "多线启动短路";
        case 0x37:
            return "多线启动断路";
        case 0x38:
            return "多线停止短路";
        case 0x39:
            return "多线停止断路";
        case 0x3B:
            return "声光通讯故障";
        case 0x3C:
            return "总线板故障";
        case 0x3E:
            return "声光启动";
        case 0x3F:
            return "声光短路";
        case 0x40:
            return "声光断路";
        default:
            return "Invalid data!";
        }
}

// 函数：解析三类报警信息部位号函数
// 输入：数据1（d14）（int）
// 输出：三类报警信息部位号（string 字符串）
string YiaiParse::transClassThreeAlarmBoardNum(int d14){
        // 将整数转换为二进制字符串，不足8位在左侧补0
        QString binaryStr = QString::number(d14, 2).rightJustified(8, '0');

        // 提取断路板号和键号
        int boardNum = binaryStr.left(2).toInt(nullptr, 2);
        int keyNum = binaryStr.right(6).toInt(nullptr, 2);
        return "版号" + to_string(boardNum)+";" + "键号" + to_string(keyNum)+";";
}


// 函数：解析其余信息函数
// 输入：无
// 输出：其他信息结构体（ClassAdditionalInfo）
ClassAdditionalInfo YiaiParse::parseClassAdditionalCode(){
        ClassAdditionalInfo classAdditionalInfo;

        classAdditionalInfo.ifValid = true;

        //判断包头、版本号、结束字节是否正确
        if( data[0] != 0x68 || data[3] != 0x68 ){
            classAdditionalInfo.ifValid = false;
            classAdditionalInfo.errorInfo = "包头错误： " + to_string(data[0]) + to_string(data[3]);
        }
        if(data[9]!=0x29){
            classAdditionalInfo.ifValid = false;
            classAdditionalInfo.errorInfo = "版本号错误： " + to_string(data[9]);
        }
        if(data[data[1]+5]!=0x16){
            classAdditionalInfo.ifValid = false;
            classAdditionalInfo.errorInfo = "结束字节错误： " + to_string(data[data[1]+5]);
        }

        //检验累加和是否正确
        int tempsum = 0;
        for(int i=0;i<data[1];i++){
            tempsum += data[i+4];
        }
        tempsum = tempsum % 256; // 只保留低八位

        if (data[data[1]+4] != tempsum){
            classAdditionalInfo.ifValid = false;
            classAdditionalInfo.errorInfo = "sum code error: " + to_string(data[data[1]+4]) + " vs " + to_string(tempsum);
            return classAdditionalInfo;
        }

        //判断代码信息
        string status;
        if( data[10]==0x8b )
            status = "报警器复位";
        else
            status = "巡检返回";
        //存入结构体
        int uNum=data[4]+data[5];
        int mNum=data[6]+data[7];
        classAdditionalInfo.unitNum = "本机号：" + to_string(uNum)+";";
        classAdditionalInfo.machineNum = "目标机号：" + to_string(mNum)+";";
        classAdditionalInfo.codeInfo = "代码信息：" + status +";";

        return classAdditionalInfo;
}

// 函数：解析应答包函数
// 输入：无
// 输出：应答包结构体（ClassResponseInfo）
ClassResponseInfo YiaiParse::parseClassResponseCode(){
        ClassResponseInfo classResponseInfo;

        classResponseInfo.ifValid = true;

        //判断包头、版本号、结束字节是否正确
        if( data[0] != 0x68 || data[3] != 0x68 ){
            classResponseInfo.ifValid = false;
            classResponseInfo.errorInfo = "包头错误： " + to_string(data[0]) + to_string(data[3]);
        }
        if(data[9]!=0x29){
            classResponseInfo.ifValid = false;
            classResponseInfo.errorInfo = "版本号错误： " + to_string(data[9]);
        }
        if(data[data[1]+5]!=0x16){
            classResponseInfo.ifValid = false;
            classResponseInfo.errorInfo = "结束字节错误： " + to_string(data[data[1]+5]);
        }

        //检验累加和是否正确
        int tempsum = 0;
        for(int i=0;i<data[1];i++){
            tempsum += data[i+4];
        }
        tempsum = tempsum % 256; // 只保留低八位

        if (data[data[1]+4] != tempsum){
            classResponseInfo.ifValid = false;
            classResponseInfo.errorInfo = "sum code error: " + to_string(data[data[1]+4]) + " vs " + to_string(tempsum);
            return classResponseInfo;
        }

        //存入结构体
        int uNum=data[4]+data[5];
        int mNum=data[6]+data[7];
        classResponseInfo.unitNum = "本机号：" + to_string(uNum)+";";
        classResponseInfo.machineNum = "目标机号：" + to_string(mNum)+";";
        classResponseInfo.responseType = "应答类型：" + to_string(data[12]) +";";

        return classResponseInfo;
}


// 函数：应答包准备函数
// 输入：无
// 输出：应答包（string 字符串）
string YiaiParse::answerPacks(){

        // 将data数组中的元素转换为十六进制字符串，并赋值给r4、r5、r6、r7、r12
        QString r4 = QString::number(data[6], 16);
        QString r5 = QString::number(data[7], 16);
        QString r6 = QString::number(data[4], 16);
        QString r7 = QString::number(data[5], 16);
        QString r12 = QString::number(data[10], 16);
        //判断转换操作是否成功
        bool ok;

        response[0]=0x68;
        response[1]=0x09;
        response[2]=0x09;
        response[3]=0x68;
        //转换为十六进制整数
        response[4]=r4.toInt(&ok, 16);
        response[5]=r5.toInt(&ok, 16);
        response[6]=r6.toInt(&ok, 16);
        response[7]=r7.toInt(&ok, 16);
        response[8]=0x00;
        response[9]=0x29;
        response[10]=0xfa;
        response[11]=0x01;
        response[12]=r12.toInt(&ok, 16);
//        qDebug()<<ok;

        //计算累加和
        int tempsum = 0;
        for(int i=4;i<13;i++){
            tempsum += response[i];
        }
        tempsum = tempsum % 256; // 只保留低八位
        QString r13 = QString::number(tempsum, 16);     //转换为十六进制字符串


        response[13]=r13.toInt(&ok, 16);
        response[14]=0x16;

        //将数组整数转换为十六进制字符串并拼成字符串
        QString str;
        for(int i=0;i<15;i++){
            str += " " + QString::number(response[i], 16).rightJustified(2, '0');       //rightJustified确保字符串的长度为两个字符，并在需要时使用前导零进行填充
        }
        return str.toStdString();
}


// 函数：刷新指令函数
// 输入：无
// 输出：读火灾报警系统报警信息即刷新命令（string 字符串）
string YiaiParse::refreshInstruction(){
        instruction[0] = 0x68;
        instruction[1] = 0x09;
        instruction[2] = 0x09;
        instruction[3] = 0x68;
        instruction[4] = 0x00;
        instruction[5] = 0x00;
        instruction[6] = 0x02;
        instruction[7] = 0x00;
        instruction[8] = 0x00;
        instruction[9] = 0x29;
        instruction[10] = 0xc8;
        instruction[11] = 0x01;
        instruction[12] = 0x00;
        //判断转换操作是否成功
        bool ok;

        //计算累加和
        int tempsum = 0;
        for(int i=4;i<13;i++){
            tempsum += instruction[i];
        }
        tempsum = tempsum % 256; // 只保留低八位
        QString r13 = QString::number(tempsum, 16);         //转换为十六进制字符串

        instruction[13] = r13.toInt(&ok, 16);
        instruction[14] = 0x16;

        //将数组整数转换为十六进制字符串并拼成字符串
        QString str;
        for(int i=0;i<15;i++){
            str += " " + QString::number(instruction[i], 16).rightJustified(2, '0');        //rightJustified确保字符串的长度为两个字符，并在需要时使用前导零进行填充
        }
        return str.toStdString();
}


// 函数：巡检指令函数
// 输入：无
// 输出：计算机巡检控制器（string 字符串）
string YiaiParse::inspectionInstruction(){
        instruction[0] = 0x68;
        instruction[1] = 0x09;
        instruction[2] = 0x09;
        instruction[3] = 0x68;
        instruction[4] = 0x00;
        instruction[5] = 0x00;
        instruction[6] = 0x02;
        instruction[7] = 0x00;
        instruction[8] = 0x00;
        instruction[9] = 0x29;
        instruction[10] = 0xce;
        instruction[11] = 0x01;
        instruction[12] = 0x00;
        //判断转换操作是否成功
        bool ok;

        //计算累加和
        int tempsum = 0;
        for(int i=4;i<13;i++){
            tempsum += instruction[i];
        }
        tempsum = tempsum % 256; // 只保留低八位
        QString r13 = QString::number(tempsum, 16);     //转换为十六进制字符串

        instruction[13] = r13.toInt(&ok, 16);
        instruction[14] = 0x16;

        //将数组整数转换为十六进制字符串并拼成字符串
        QString str;
        for(int i=0;i<15;i++){
            str += " " + QString::number(instruction[i], 16).rightJustified(2, '0');    //rightJustified确保字符串的长度为两个字符，并在需要时使用前导零进行填充
        }
        return str.toStdString();
}
