#ifndef YIAIPARSE_H
#define YIAIPARSE_H
#define ClassOneAlarmInfo YiaiOne
#define ClassTwoAlarmInfo YiaiTwo
#include <string>
#include <algorithm>
#include <iostream>
#include <QtCore>
#include <QDebug>

using namespace std;

typedef struct {

    string errorInfo;       //警报信息
    string unitNum;         //本机号
    string machineNum;      //目标机号
    string objectNum;       //对象数目
    string circuitNum;      //回路
    string address;         //地址
    string alarmType;       //报警类型
    string changeStatus;    //状态改变量
    string hour;
    string minute;
    string reservedData[8]; //32字节保留数据
    bool ifValid;


    string toString(){
        if(ifValid){
            return unitNum+" "+
                   machineNum+" "+
                   objectNum+" "+
                   circuitNum+" "+
                   address+" "+
                   alarmType+" "+
                   changeStatus+" "+
                   hour+" "+
                   minute;
        }else{
            return "Invalid data! " + errorInfo;
        }
    }
} ClassOneAlarmInfo;


typedef struct {

    string errorInfo;       //警报信息
    string unitNum;         //本机号
    string machineNum;      //目标机号
    string objectNum;       //对象数目
    string circuitNum;      //回路
    string alarmType;       //报警类型
    string number;          //机号
    string hour;
    string minute;
    bool ifValid;


    string toString(){
        if(ifValid){
            return unitNum+" "+
                   machineNum+" "+
                   objectNum+" "+
                   number+" "+
                   circuitNum+" "+
                   alarmType+" "+
                   hour+" "+
                   minute;
        }else{
            return "Invalid data! " + errorInfo;
        }
    }
} ClassTwoAlarmInfo;

typedef struct {

    string errorInfo;       //警报信息
    string unitNum;         //本机号
    string machineNum;      //目标机号
    string objectNum;       //对象数目
    string boardNum;        //板号／键号／区号
    string faultType;       //故障类型
    string number;          //机号
    string hour;
    string minute;
    bool ifValid;


    string toString(){
        if(ifValid){
            return unitNum+" "+
                   machineNum+" "+
                   objectNum+" "+
                   number+" "+
                   boardNum+" "+
                   faultType+" "+
                   hour+" "+
                   minute;
        }else{
            return "Invalid data! " + errorInfo;
        }
    }
} ClassThreeAlarmInfo;

typedef struct {

    string errorInfo;       //警报信息
    string codeInfo;        //代码信息
    string unitNum;         //本机号
    string machineNum;      //目标机号
    bool ifValid;


    string toString(){
        if(ifValid){
            return codeInfo+" "+
                   unitNum+" "+
                   machineNum+" 对象数目 ：1";
        }else{
            return "Invalid data! " + errorInfo;
        }
    }
} ClassAdditionalInfo;


typedef struct {

    string errorInfo;       //警报信息
    string responseType;    //应答类型
    string unitNum;         //本机号
    string machineNum;      //目标机号
    bool ifValid;


    string toString(){
        if(ifValid){
            return responseType+" "+
                   unitNum+" "+
                   machineNum+" 对象数目 ：1";
        }else{
            return "Invalid data! " + errorInfo;
        }
    }
} ClassResponseInfo;



class YiaiParse
{
public:
    YiaiParse();

    // 函数：解析信息函数
    // 输入：接收的信息（char* 字符串）
    // 输出：解析的信息（string 字符串）
    string parse(const char* info);

    // 函数：解析一类报警信息函数
    // 输入：无
    // 输出：一类报警信息结构体（ClassOneAlarmInfo）
    ClassOneAlarmInfo parseClassOneAlarmCode();

    // 函数：解析二类报警信息函数
    // 输入：无
    // 输出：二类报警信息结构体（ClassTwoAlarmInfo）
    ClassTwoAlarmInfo parseClassTwoAlarmCode();

    // 函数：解析三类故障信息函数
    // 输入：无
    // 输出：三类故障信息结构体（ClassThreeAlarmInfo）
    ClassThreeAlarmInfo parseClassThreeAlarmCode();

    // 函数：解析其余信息函数
    // 输入：无
    // 输出：其他信息结构体（ClassAdditionalInfo）
    ClassAdditionalInfo parseClassAdditionalCode();

    // 函数：解析应答包函数
    // 输入：无
    // 输出：应答包结构体（ClassResponseInfo）
    ClassResponseInfo parseClassResponseCode();

    // 函数：解析一类报警信息报警代码函数
    // 输入：数据1（d14）（int）
    // 输出：一类报警信息报警代码（string 字符串）
    string transClassOneAlarmCodeInfo(int d14);

    // 函数：解析二类报警信息报警代码函数
    // 输入：数据1（d12）（int）
    // 输出：二类报警信息报警代码（string 字符串）
    string transClassTwoAlarmCodeInfo(int d12);

    // 函数：解析三类报警信息报警代码函数
    // 输入：数据1（d12）（int）
    // 输出：三类报警信息报警代码（string 字符串）
    string transClassThreeAlarmCodeInfo(int d12);

    // 函数：解析三类报警信息部位号函数
    // 输入：数据1（d14）（int）
    // 输出：三类报警信息部位号（string 字符串）
    string transClassThreeAlarmBoardNum(int d14);

    // 函数：应答包准备函数
    // 输入：无
    // 输出：应答包（string 字符串）
    string answerPacks();

    // 函数：刷新指令函数
    // 输入：无
    // 输出：读火灾报警系统报警信息即刷新命令（string 字符串）
    string refreshInstruction();

    // 函数：巡检指令函数
    // 输入：无
    // 输出：计算机巡检控制器（string 字符串）
    string inspectionInstruction();



private:
    int * data;
    int *response;
    int *instruction;
};

#endif // YIAIPARSE_H
