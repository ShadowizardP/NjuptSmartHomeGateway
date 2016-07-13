#include <math.h>
//#include "Cluster.h"
#include "SmartHome.h"
#include "support.h"


#define TD 1800

THData getTHData();    //�����ʪ������
short THDataIsEmpty();    //�жϴ������
THData findTHDataByTime(zigbeeTime time);    //����ʱ���ѯ����
THData getEarlyTHData(zigbeeTime time);    //��ȡ�Ƚ�ʱ�������
void saveCluster(cluster cl[12]);    //�洢��۽��
cluster computeCluster(cluster oc, THData new_Data);    //��ۼ���
void clusterProgress();    //��۹���

cluster getCluster(int flag);    //��ȡ��
double getDistance(cluster oc, THData new_Data);    //��ȡ�������ĵľ���
short isOutlier(THData data, cluster c);    //�ж��Ƿ�Ϊ�쳣ֵ
void deleteTHData(THData data);    //ɾ���쳣ֵ
void saveTHData(THData data);    //�洢����ֵ


THData getTHData()    //�����ʪ������
{
	THData data = { 0 };
	return data;
}

short THDataIsEmpty()    //�жϴ������
{
	return 0;
}

THData findTHDataByTime(zigbeeTime time)    //����ʱ���ѯ����
{
	THData data = { 0 };
	return data;
}

THData getEarlyTHData(zigbeeTime time)    //��ȡ�Ƚ�ʱ�������
{
	zigbeeTime et = secondToTime(timeToSecond(time) - TD);
	return findTHDataByTime(et);
}


cluster computeCluster(cluster oc, THData new_Data)    //��ۼ���
{
	THData early_Data = getEarlyTHData(new_Data.time);    //��ȡ�Ƚ�ʱ�������
	double changeT = new_Data.temperature - early_Data.temperature;    //��������Ĳ�ֵ���ݵ�
	double changeH = new_Data.humidity - early_Data.humidity;
	double distance = sqrt((changeT - oc.changeT) * (changeT - oc.changeT) + (changeH - oc.changeH) * (changeH - oc.changeH));    //�������ݵ㵽���ĵľ���
	oc.avgDistance = (distance + oc.dataNumber * oc.avgDistance) / (oc.dataNumber + 1);    //���㵽���ĵ�ƽ������
	oc.changeT = (changeT + oc.changeT * oc.dataNumber) / (oc.dataNumber + 1);
	oc.changeH = (changeH + oc.changeH * oc.dataNumber) / (oc.dataNumber + 1);
	distance = sqrt((changeT - oc.changeT) * (changeT - oc.changeT) + (changeH - oc.changeH) * (changeH - oc.changeH));    //���㵽�����ĵľ���
	if (distance > oc.maxDistance)    //�ж�������
	{
		oc.maxDistance = distance;
	}
	oc.outlierdistance = 0.5 * oc.maxDistance + 0.5 * (2 * oc.avgDistance);    //�������Χ
	return oc;
}

void clusterProgress()    //��۹���
{
	//cluster clusterArray[12];    //������۵Ĵ�
	cluster currentCluster;
	int flag = 0;    //��ʶ��ǰ����������
	while (!THDataIsEmpty())    //��ʼ��۹���
	{
		THData data;
		data = getTHData();
		switch (data.time.month)    //����������
		{
		case 12:
		case 1:
		case 2:
		{
				  if (data.time.hour <= 7)
				  {
					  flag = 0;
				  }
				  else if (8 <= data.time.hour && data.time.hour <= 14)
				  {
					  flag = 1;
				  }
				  else
				  {
					  flag = 2;
				  }
		}
			break;
		case 3:
		case 4:
		case 5:
		{
				  if (data.time.hour <= 6)
				  {
					  flag = 3;
				  }
				  else if (8 <= data.time.hour && data.time.hour <= 14)
				  {
					  flag = 4;
				  }
				  else
				  {
					  flag = 5;
				  }
		}
			break;
		case 6:
		case 7:
		case 8:
		{
				  if (data.time.hour <= 5)
				  {
					  flag = 6;
				  }
				  else if (8 <= data.time.hour && data.time.hour <= 14)
				  {
					  flag = 7;
				  }
				  else
				  {
					  flag = 8;
				  }
		}
			break;
		case 9:
		case 10:
		case 11:
		{
				   if (data.time.hour <= 6)
				   {
					   flag = 9;
				   }
				   else if (8 <= data.time.hour && data.time.hour <= 14)
				   {
					   flag = 10;
				   }
				   else
				   {
					   flag = 11;
				   }
		}
			break;
		default:
			break;
		}
		currentCluster = THClusterData[flag];    //��ȡ������
		THClusterData[flag] = computeCluster(currentCluster, data);    //����������
	}

}


cluster getCluster(int flag)    //��ȡ��
{
	cluster c = { 0 };
	return c;
}

double getDistance(cluster oc, THData new_Data)    //��ȡ�������ĵľ���
{
	THData early_Data = getEarlyTHData(new_Data.time);
	double changeT = new_Data.temperature - early_Data.temperature;
	double changeH = new_Data.humidity - early_Data.humidity;
	double distance = sqrt((changeT - oc.changeT) * (changeT - oc.changeT) + (changeH - oc.changeH) * (changeH - oc.changeH));
	return distance;
}

short isOutlier(THData data, cluster c)    //�ж��Ƿ�Ϊ�쳣ֵ
{
	if (getDistance(c, data) > c.outlierdistance)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void deleteTHData(THData data)    //ɾ���쳣ֵ
{

}

void saveTHData(THData data)    //�洢����ֵ
{

}

int testOutlierProgress()    //�ж��쳣ֵ����
{
	int success = 0;
	int change = OriginalTemperatureData[9].temperature - OriginalTemperatureData[8].temperature;
	//cout<<"9:"<<OriginalTemperatureData[8].temperature<<"      10:"<<OriginalTemperatureData[9].temperature<<endl;
	if(change < 5 && change > -5)
	{
	    success = 1;
	}
/*
	//cluster clusterArray[12];    //������۵Ĵ�
	//for (int i = 0; i < 12; i++)    //��ȡ���д�
	//{
	//	clusterArray[i] = getCluster(i);
	//}
	cluster currentCluster;
	int flag = 0;    //��ʶ��ǰ����������
	while (!THDataIsEmpty())    //��ʼ�ж��쳣ֵ����
	{
		THData data;
		data = getTHData();
		switch (data.time.month)    //����������
		{
		case 12:
		case 1:
		case 2:
		{
				  if (data.time.hour <= 7)
				  {
					  flag = 0;
				  }
				  else if (8 <= data.time.hour && data.time.hour <= 14)
				  {
					  flag = 1;
				  }
				  else
				  {
					  flag = 2;
				  }
		}
			break;
		case 3:
		case 4:
		case 5:
		{
				  if (data.time.hour <= 6)
				  {
					  flag = 3;
				  }
				  else if (8 <= data.time.hour && data.time.hour <= 14)
				  {
					  flag = 4;
				  }
				  else
				  {
					  flag = 5;
				  }
		}
			break;
		case 6:
		case 7:
		case 8:
		{
				  if (data.time.hour <= 5)
				  {
					  flag = 6;
				  }
				  else if (8 <= data.time.hour && data.time.hour <= 14)
				  {
					  flag = 7;
				  }
				  else
				  {
					  flag = 8;
				  }
		}
			break;
		case 9:
		case 10:
		case 11:
		{
				   if (data.time.hour <= 6)
				   {
					   flag = 9;
				   }
				   else if (8 <= data.time.hour && data.time.hour <= 14)
				   {
					   flag = 10;
				   }
				   else
				   {
					   flag = 11;
				   }
		}
			break;
		default:
			break;
		}

		//currentCluster = clusterArray[flag];   //��ȡ������
		currentCluster = THClusterData[flag];
		if (isOutlier(data, currentCluster))    //������쳣ֵ��ɾ������
		{
			success = 0;
			deleteTHData(data);
		}
		else
		{
			success = 1;
			saveTHData(data);    //������ֵ��洢����
			THClusterData[flag] = computeCluster(currentCluster, data);    //����������
		}
	}
*/
	return success;
}
