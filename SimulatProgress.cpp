
#include <stdlib.h>
#include "Simulate.h"

void simulateResult(float xData[], float yData[], float xPoint[], float yRes[], int resCount);    //��ϼ������
void getDataPoints(float xData[], float yData[]);    //��ȡ������ϵ����ݵ�
void getTimeNeed(float x[]);    //��ȡ��Ҫ�����ʱ���
int getTimeNeedCount();    //��ȡ��Ҫ�����ʱ���ĸ���

void simulateResult(float xData[], float yData[], float xPoint[], float yRes[], int resCount)    //��ϼ������
{
	float result[T + 1];    //���ϵ��
	simulateProcess(xData, yData, result);    //��Ϲ���
	for (int i = 0; i < resCount; i++)
	{
		yRes[i] = result[0] + result[1] * xPoint[i] + result[2] * xPoint[i] * xPoint[i] + result[3] * xPoint[i] * xPoint[i] * xPoint[i];    //������Ͻ���������ݵ�
	}
}

void getDataPoints(float xData[], float yData[])    //��ȡ������ϵ����ݵ�
{

}

void getTimeNeed(float x[])    //��ȡ��Ҫ�����ʱ���
{

}

int getTimeNeedCount()    //��ȡ��Ҫ�����ʱ���ĸ���
{
	return 3;
}

void getSimulateResult()    //��ȡ��ϼ�����
{
	float timeData[N];
	float temperatureData[N];
	getDataPoints(timeData, temperatureData);    //��ȡ������ϵ����ݵ�
	int resCount = getTimeNeedCount();    //��ȡ��Ҫ�����ʱ���ĸ���
	float *timePoint;    //����ָ��
	float *temperatureRes;
	timePoint = (float *)malloc(sizeof(float)*resCount);    //������Ҫ�����ʱ��������̬�����ڴ�ռ�
	temperatureRes = (float *)malloc(sizeof(float)*resCount);
	getTimeNeed(timePoint);    //��ȡ��Ҫ�����ʱ���
	simulateResult(timeData, temperatureData, timePoint, temperatureRes, resCount);   //��ϼ������
}