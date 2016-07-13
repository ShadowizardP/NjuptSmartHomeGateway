
#define PERIOD 30

#include <iostream>
#include <sys/stat.h>  

using namespace std;

typedef struct zigbeeTime    //节点接收的传感器时间结构体
{
	unsigned short year;
	unsigned short month;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
}zigbeeTime;

typedef struct oTemperatureData    //原始温度数据结构体
{
	zigbeeTime time;
	double temperature;
}oTemperatureData;

typedef struct oHumidityData    //原始湿度数据结构体
{
	zigbeeTime time;
	double humidity;
}oHumidityData;

typedef struct temperatureData    //转换为二维模型的温度数据结构体
{
	int time;
	double temperature;
}temperatureData;

typedef struct humidityData    //转换为二维模型的湿度数据结构体
{
	int time;
	double humidity;
}humidityData;


typedef struct THData    //温湿度数据
{
	zigbeeTime time;
	double temperature;
	double humidity;
}THData;

typedef struct cluster    //类聚的簇
{
	double changeT;
	double changeH;
	int dataNumber;
	double avgDistance;
	double maxDistance;
	double outlierdistance;
}cluster;

extern oTemperatureData OriginalTemperatureData[10];
extern oHumidityData OriginalHumidityData[10];
extern THData TemperatureHumidityData[10];
extern cluster THClusterData[12];
extern int isSnaping;

short isRunNian(short year);    //判断闰年
short daysInMonth(short month, short year);    //获取每月天数
short getFirstYear();    //获取初始年份
int timeToSecond(zigbeeTime ot);    //计算初始年份开始的第n秒
zigbeeTime  secondToTime(int t);    //将初始年份开始第n秒恢复为正常时间格式
string zigbeeTimeToString(zigbeeTime time);
short isFull();    //判断存储空间是否紧张
//int CheckDir(char* Dir);
int mkpath(std::string s,mode_t mode);
