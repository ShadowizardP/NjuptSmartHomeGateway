
#define PERIOD 30

#include <iostream>
#include <sys/stat.h>  

using namespace std;

typedef struct zigbeeTime    //�ڵ���յĴ�����ʱ��ṹ��
{
	unsigned short year;
	unsigned short month;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
}zigbeeTime;

typedef struct oTemperatureData    //ԭʼ�¶����ݽṹ��
{
	zigbeeTime time;
	double temperature;
}oTemperatureData;

typedef struct oHumidityData    //ԭʼʪ�����ݽṹ��
{
	zigbeeTime time;
	double humidity;
}oHumidityData;

typedef struct temperatureData    //ת��Ϊ��άģ�͵��¶����ݽṹ��
{
	int time;
	double temperature;
}temperatureData;

typedef struct humidityData    //ת��Ϊ��άģ�͵�ʪ�����ݽṹ��
{
	int time;
	double humidity;
}humidityData;


typedef struct THData    //��ʪ������
{
	zigbeeTime time;
	double temperature;
	double humidity;
}THData;

typedef struct cluster    //��۵Ĵ�
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

short isRunNian(short year);    //�ж�����
short daysInMonth(short month, short year);    //��ȡÿ������
short getFirstYear();    //��ȡ��ʼ���
int timeToSecond(zigbeeTime ot);    //�����ʼ��ݿ�ʼ�ĵ�n��
zigbeeTime  secondToTime(int t);    //����ʼ��ݿ�ʼ��n��ָ�Ϊ����ʱ���ʽ
string zigbeeTimeToString(zigbeeTime time);
short isFull();    //�жϴ洢�ռ��Ƿ����
//int CheckDir(char* Dir);
int mkpath(std::string s,mode_t mode);