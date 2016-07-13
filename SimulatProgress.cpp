
#include <stdlib.h>
#include "Simulate.h"

void simulateResult(float xData[], float yData[], float xPoint[], float yRes[], int resCount);    //拟合计算过程
void getDataPoints(float xData[], float yData[]);    //获取用于拟合的数据点
void getTimeNeed(float x[]);    //获取需要计算的时间点
int getTimeNeedCount();    //获取需要计算的时间点的个数

void simulateResult(float xData[], float yData[], float xPoint[], float yRes[], int resCount)    //拟合计算过程
{
	float result[T + 1];    //结果系数
	simulateProcess(xData, yData, result);    //拟合过程
	for (int i = 0; i < resCount; i++)
	{
		yRes[i] = result[0] + result[1] * xPoint[i] + result[2] * xPoint[i] * xPoint[i] + result[3] * xPoint[i] * xPoint[i] * xPoint[i];    //根据拟合结果计算数据点
	}
}

void getDataPoints(float xData[], float yData[])    //获取用于拟合的数据点
{

}

void getTimeNeed(float x[])    //获取需要计算的时间点
{

}

int getTimeNeedCount()    //获取需要计算的时间点的个数
{
	return 3;
}

void getSimulateResult()    //获取拟合计算结果
{
	float timeData[N];
	float temperatureData[N];
	getDataPoints(timeData, temperatureData);    //获取用于拟合的数据点
	int resCount = getTimeNeedCount();    //获取需要计算的时间点的个数
	float *timePoint;    //定义指针
	float *temperatureRes;
	timePoint = (float *)malloc(sizeof(float)*resCount);    //根据需要计算的时间点个数动态分配内存空间
	temperatureRes = (float *)malloc(sizeof(float)*resCount);
	getTimeNeed(timePoint);    //获取需要计算的时间点
	simulateResult(timeData, temperatureData, timePoint, temperatureRes, resCount);   //拟合计算过程
}