#include <math.h>
//#include "Cluster.h"
#include "SmartHome.h"
#include "support.h"


#define TD 1800

THData getTHData();    //获得温湿度数据
short THDataIsEmpty();    //判断处理完毕
THData findTHDataByTime(zigbeeTime time);    //根据时间查询数据
THData getEarlyTHData(zigbeeTime time);    //获取比较时间点数据
void saveCluster(cluster cl[12]);    //存储类聚结果
cluster computeCluster(cluster oc, THData new_Data);    //类聚计算
void clusterProgress();    //类聚过程

cluster getCluster(int flag);    //获取簇
double getDistance(cluster oc, THData new_Data);    //获取距离质心的距离
short isOutlier(THData data, cluster c);    //判断是否为异常值
void deleteTHData(THData data);    //删除异常值
void saveTHData(THData data);    //存储正常值


THData getTHData()    //获得温湿度数据
{
	THData data = { 0 };
	return data;
}

short THDataIsEmpty()    //判断处理完毕
{
	return 0;
}

THData findTHDataByTime(zigbeeTime time)    //根据时间查询数据
{
	THData data = { 0 };
	return data;
}

THData getEarlyTHData(zigbeeTime time)    //获取比较时间点数据
{
	zigbeeTime et = secondToTime(timeToSecond(time) - TD);
	return findTHDataByTime(et);
}


cluster computeCluster(cluster oc, THData new_Data)    //类聚计算
{
	THData early_Data = getEarlyTHData(new_Data.time);    //获取比较时间点数据
	double changeT = new_Data.temperature - early_Data.temperature;    //计算所需的差值数据点
	double changeH = new_Data.humidity - early_Data.humidity;
	double distance = sqrt((changeT - oc.changeT) * (changeT - oc.changeT) + (changeH - oc.changeH) * (changeH - oc.changeH));    //计算数据点到质心的距离
	oc.avgDistance = (distance + oc.dataNumber * oc.avgDistance) / (oc.dataNumber + 1);    //计算到质心的平均距离
	oc.changeT = (changeT + oc.changeT * oc.dataNumber) / (oc.dataNumber + 1);
	oc.changeH = (changeH + oc.changeH * oc.dataNumber) / (oc.dataNumber + 1);
	distance = sqrt((changeT - oc.changeT) * (changeT - oc.changeT) + (changeH - oc.changeH) * (changeH - oc.changeH));    //计算到新质心的距离
	if (distance > oc.maxDistance)    //判断最大距离
	{
		oc.maxDistance = distance;
	}
	oc.outlierdistance = 0.5 * oc.maxDistance + 0.5 * (2 * oc.avgDistance);    //计算合理范围
	return oc;
}

void clusterProgress()    //类聚过程
{
	//cluster clusterArray[12];    //定义类聚的簇
	cluster currentCluster;
	int flag = 0;    //标识当前数据所属簇
	while (!THDataIsEmpty())    //开始类聚过程
	{
		THData data;
		data = getTHData();
		switch (data.time.month)    //决策树分类
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
		currentCluster = THClusterData[flag];    //获取所属簇
		THClusterData[flag] = computeCluster(currentCluster, data);    //更新所属簇
	}

}


cluster getCluster(int flag)    //获取簇
{
	cluster c = { 0 };
	return c;
}

double getDistance(cluster oc, THData new_Data)    //获取距离质心的距离
{
	THData early_Data = getEarlyTHData(new_Data.time);
	double changeT = new_Data.temperature - early_Data.temperature;
	double changeH = new_Data.humidity - early_Data.humidity;
	double distance = sqrt((changeT - oc.changeT) * (changeT - oc.changeT) + (changeH - oc.changeH) * (changeH - oc.changeH));
	return distance;
}

short isOutlier(THData data, cluster c)    //判断是否为异常值
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

void deleteTHData(THData data)    //删除异常值
{

}

void saveTHData(THData data)    //存储正常值
{

}

int testOutlierProgress()    //判断异常值过程
{
	int success = 0;
	int change = OriginalTemperatureData[9].temperature - OriginalTemperatureData[8].temperature;
	//cout<<"9:"<<OriginalTemperatureData[8].temperature<<"      10:"<<OriginalTemperatureData[9].temperature<<endl;
	if(change < 5 && change > -5)
	{
	    success = 1;
	}
/*
	//cluster clusterArray[12];    //定义类聚的簇
	//for (int i = 0; i < 12; i++)    //获取所有簇
	//{
	//	clusterArray[i] = getCluster(i);
	//}
	cluster currentCluster;
	int flag = 0;    //标识当前数据所属簇
	while (!THDataIsEmpty())    //开始判断异常值过程
	{
		THData data;
		data = getTHData();
		switch (data.time.month)    //决策树分类
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

		//currentCluster = clusterArray[flag];   //获取所属簇
		currentCluster = THClusterData[flag];
		if (isOutlier(data, currentCluster))    //如果是异常值则删除数据
		{
			success = 0;
			deleteTHData(data);
		}
		else
		{
			success = 1;
			saveTHData(data);    //是正常值则存储数据
			THClusterData[flag] = computeCluster(currentCluster, data);    //更新所属簇
		}
	}
*/
	return success;
}
