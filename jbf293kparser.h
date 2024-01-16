#ifndef JBF293KPARSER_H
#define JBF293KPARSER_H
#define ClassOneAlarmInfo QingniaoOne
#define ClassTwoAlarmInfo QingniaoTwo
#include <string>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <QtCore>

using namespace std;

typedef struct {
    // 记录报错信息
    string errorInfo;
    string codeInfo;
    string machineNum;
    string circuitNum;
    string partNum;
    string partType;
    string year;
    string month;
    string day;
    string hour;
    string miniute;
    string second;
    bool ifValid;

    string toString(){
        if(ifValid){
            return codeInfo+" "+
                   machineNum+" "+
                   circuitNum+" "+
                   partNum+" "+
                   partType+" "+
                   year+" "+
                   month+" "+
                   day+" "+
                   hour+" "+
                   miniute+" "+
                   second;
        }else{
            return "Invalid data! " + errorInfo;
        }
    }
} ClassOneAlarmInfo;


typedef struct {
    // 记录报错信息
    string errorInfo;
    string codeInfo;
    string machineNum;
    string partNum;
    string year;
    string month;
    string day;
    string hour;
    string miniute;
    string second;
    bool ifValid;

    string toString(){
        if(ifValid){
            return codeInfo+" "+
                   machineNum+" "+
                   partNum+" "+
                   year+" "+
                   month+" "+
                   day+" "+
                   hour+" "+
                   miniute+" "+
                   second;
        }else{
            return "Invalid data! " + errorInfo;
        }
    }
} ClassTwoAlarmInfo;

typedef struct {
    // 记录报错信息
    string errorInfo;
    string codeInfo;
    string machineNum;
    string circuitNum;
    string partNum;
    string partType;
    string year;
    string month;
    string day;
    string hour;
    string miniute;
    string second;
    string sensorChannelNum;
    string alarmValue;
    bool ifValid;
    bool ifSpecialElectricalFire;

    string toString(){
        if(ifValid){
            if (ifSpecialElectricalFire){
                return codeInfo+" "+
                       machineNum+" "+
                       circuitNum+" "+
                       partNum+" "+
                       partType+" "+
                       sensorChannelNum+" "+
                       alarmValue+" "+
                       year+" "+
                       month+" "+
                       day+" "+
                       hour+" "+
                       miniute+" "+
                       second;
            }else{
                return codeInfo+" "+
                       machineNum+" "+
                       circuitNum+" "+
                       partNum+" "+
                       partType+" "+
                       year+" "+
                       month+" "+
                       day+" "+
                       hour+" "+
                       miniute+" "+
                       second;
            }
        }else{
            return "Invalid data! " + errorInfo;
        }
    }
} ClassFireDoorAndElectricalFireAlarmInfo;



class JBF293KParser
{
public:
    JBF293KParser();


    // 作者：刘陈林
    // 时间：20230817
    // 函数：解析信息函数
    // 输入：接收的信息（char* 字符串）
    // 输出：解析的信息（string 字符串）
    string parse(const char* info);


    // 作者：刘陈林
    // 时间：20230817
    // 函数：解析一类报警信息函数
    // 输入：无
    // 输出：一类报警信息结构体（ClassOneAlarmInfo）
    ClassOneAlarmInfo parseClassOneAlarmCode();


    // 作者：刘陈林
    // 时间：20230817
    // 函数：解析二类报警信息函数
    // 输入：无
    // 输出：二类报警信息结构体（ClassTwoAlarmInfo）
    ClassTwoAlarmInfo parseClassTwoAlarmCode();


    // 作者：刘陈林
    // 时间：20230817
    // 函数：解析防火门与电气火灾报警信息函数
    // 输入：无
    // 输出：防火门与电气火灾报警信息结构体（ClassFireDoorAndElectricalFireAlarmInfo）
    ClassFireDoorAndElectricalFireAlarmInfo parseClassFireDoorAndElectricalFireAlarmCode();


    // 作者：刘陈林
    // 时间：20230817
    // 函数：解析一类报警信息报警代码函数
    // 输入：数据1（d1）（int）
    // 输出：一类报警信息报警代码（string 字符串）
    string transClassOneAlarmCodeInfo(int d1);


    // 作者：刘陈林
    // 时间：20230817
    // 函数：解析一类报警信息部位号函数
    // 输入：数据1、数据4（d1, d4）（int, int）
    // 输出：一类报警信息部位号（string 字符串）
    string transClassOneAlarmPartNum(int d1, int d4);


    // 作者：许凯
    // 时间：20230817
    // 函数：解析二类报警信息报警代码函数
    // 输入：数据1、数据3（d1, d3）（int, int）
    // 输出：二类报警信息报警代码（string 字符串）
    string transClassTwoAlarmCodeInfo(int d1,int d3);


    // 作者：刘陈林
    // 时间：20230817
    // 函数：解析二类报警信息部位号函数
    // 输入：数据1、数据4（d1, d4）（int, int）
    // 输出：二类报警信息部位号（string 字符串）
    string transClassTwoAlarmPartNum(int d1, int d4);


    // 作者：许凯
    // 时间：20230817
    // 函数：解析防火门报警信息部件类型函数
    // 输入：无
    // 输出：防火门报警信息部件类型（string 字符串）
    string transClassFireDoorAlarmPartType();


    // 作者：许凯
    // 时间：20230817
    // 函数：解析电气火灾报警信息部件类型函数
    // 输入：无
    // 输出：电气火灾报警信息部件类型（string 字符串）
    string transClassElectricalFireAlarmPartType();


    // 作者：许凯
    // 时间：20230817
    // 函数：解析特殊电气火灾报警值函数
    // 输入：数据13、数据16（d13, d16）（int, int）
    // 输出：特殊电气火灾报警值（string 字符串）
    string transClassSpecialElectricalFireAlarmAlarmValue(int d13,int d16);


    // 作者：许凯
    // 时间：20230817
    // 函数：解析特殊电气火灾报警信息部件类型函数
    // 输入：无
    // 输出：防火门报警信息部件类型（string 字符串）
    string transClassSpecialElectricalFireAlarmPartType();
    QByteArray calculateCrc8(const QByteArray& data);
private:
    int * data;
    int * classOneAlarm;
    int * classTwoAlarm;
};

#endif // JBF293KPARSER_H
