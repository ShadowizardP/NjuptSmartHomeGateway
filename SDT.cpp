
#include "support.h"
#include "SmartHome.h"

#define TE 0.5  //温度旋转门宽度
#define HE 5  //湿度旋转门宽度


oTemperatureData getOTemperature();	//获取原始温度数据
oHumidityData getOHumidity();	//获取原始湿度数据
temperatureData getTemperature();	//获取新温度数据
humidityData getHumidity();	//获取新湿度数据
void saveTemperatureData(temperatureData first_temperature, temperatureData last_temperature);	//存储压缩后的温度数据
void saveHumidityData(humidityData first_humidity, humidityData last_humidity);	//存储压缩后的湿度数据
temperatureData SDTTemperatureData(temperatureData first_temperature);    //旋转门压缩温度数据
humidityData SDTHumidityData(humidityData first_humidity);   //旋转门压缩湿度数据
void sendData();    //发送数据

int getPeriod();    //获取数据采集间隔
short isEmpty();    //判断是否解压完成
void saveOTemperatureData(oTemperatureData data);    //存储解压缩后的温度数据
void saveOHumidityData(oHumidityData data);    //存储解压缩后的湿度
temperatureData uSDTTemperatureData(temperatureData first_temperature);    //解压缩温度数据
humidityData uSDTHumidityData(humidityData first_humidity);    //解压缩湿度数据

oTemperatureData getOTemperature()	//获取原始温度数据
{
	oTemperatureData temperature = { { 2015, 12, 20, 16, 38, 0 }, 20.0 };

	return temperature;
}

oHumidityData getOHumidity()	//获取原始湿度数据
{
	oHumidityData humidity = { { 2015, 12, 20, 16, 38, 0 }, 20.0 };

	return humidity;
}


temperatureData getTemperature()	//获取新温度数据
{
	temperatureData temperature;
	oTemperatureData oTemperature = getOTemperature();
	temperature.temperature = oTemperature.temperature;
	temperature.time = timeToSecond(oTemperature.time);

	return temperature;
}

humidityData getHumidity()	//获取新湿度数据
{
	humidityData humidity;
	oHumidityData oHumidity = getOHumidity();
	humidity.humidity = oHumidity.humidity;
	humidity.time = timeToSecond(oHumidity.time);

	return humidity;
}

void saveTemperatureData(temperatureData first_temperature, temperatureData last_temperature)	//存储压缩后的温度数据
{

}

void saveHumidityData(humidityData first_humidity, humidityData last_humidity)	//存储压缩后的湿度数据
{

}

temperatureData SDTTemperatureData(temperatureData first_temperature)    //旋转门压缩温度数据
{
	temperatureData last_temperature = getTemperature();    //获取初始终点
	double gateTop = first_temperature.temperature + TE;    //门上限
	double gateBottom = first_temperature.temperature - TE;    //门下限
	double fTop = (last_temperature.temperature - gateTop) / (last_temperature.time - first_temperature.time);    //获取初始上门斜率
	double fBottom = (last_temperature.temperature - gateBottom) / (last_temperature.time - first_temperature.time);    //获取初始下门斜率
	while (1)
	{
		temperatureData new_temperature = getTemperature();    //获取新接收数据

		double fTopNew = (new_temperature.temperature - gateTop) / (new_temperature.time - first_temperature.time);    //计算新上门斜率
		double fBottomNew = (new_temperature.temperature - gateBottom) / (new_temperature.time - first_temperature.time);    //计算新下门斜率
		//判断是否继续开门
		if (fTopNew > fTop)
		{
			fTop = fTopNew;
		}
		if (fBottomNew < fBottom)
		{
			fBottom = fBottomNew;
		}
		if (fTop >= fBottom)    //判断门是否打开
		{
			//打开则本次压缩完成
			saveTemperatureData(first_temperature, last_temperature);
			return new_temperature;
		}
		else
		{
			//未打开则继续压缩
			last_temperature = new_temperature;
		}
	}
}



humidityData SDTHumidityData(humidityData first_humidity)    //旋转门压缩湿度数据
{
	humidityData last_humidity = getHumidity();    //获取初始终点
	double gateTop = first_humidity.humidity + HE;    //门上限
	double gateBottom = first_humidity.humidity - HE;    //门下限
	double fTop = (last_humidity.humidity - gateTop) / (last_humidity.time - first_humidity.time);    //获取初始上门斜率
	double fBottom = (last_humidity.humidity - gateBottom) / (last_humidity.time - first_humidity.time);    //获取初始下门斜率
	while (1)
	{
		humidityData new_humidity = getHumidity();    //获取新接收数据

		double fTopNew = (new_humidity.humidity - gateTop) / (new_humidity.time - first_humidity.time);    //计算新上门斜率
		double fBottomNew = (new_humidity.humidity - gateBottom) / (new_humidity.time - first_humidity.time);    //计算新下门斜率
		//判断是否继续开门
		if (fTopNew > fTop)
		{
			fTop = fTopNew;
		}
		if (fBottomNew < fBottom)
		{
			fBottom = fBottomNew;
		}
		if (fTop >= fBottom)    //判断门是否打开
		{
			//打开则本次压缩完成
			saveHumidityData(first_humidity, last_humidity);
			return new_humidity;
		}
		else
		{
			//未打开则继续压缩
			last_humidity = new_humidity;
		}
	}
}

void sendData()    //发送数据
{

}


void dataCompress()    //压缩数据
{
	//获取初始起点
	temperatureData first_temperature = getTemperature();
	humidityData first_humidity = getHumidity();
	while (1)
	{
		if (isFull())    //若存储空间紧张则发送数据
		{
			sendData();
		}
		//继续压缩
		first_temperature = SDTTemperatureData(first_temperature);
		first_humidity = SDTHumidityData(first_humidity);
	}
}

int getPeriod()    //获取数据采集间隔
{
	return 1800;
}

short isEmpty()    //判断是否解压完成
{
	return 0;
}

void saveOTemperatureData(oTemperatureData data)    //存储解压缩后的温度数据
{

}

void saveOHumidityData(oHumidityData data)    //存储解压缩后的湿度
{

}

temperatureData uSDTTemperatureData(temperatureData first_temperature)    //解压缩温度数据
{
	temperatureData last_temperature = getTemperature();
	double f = (last_temperature.temperature - first_temperature.temperature) / (last_temperature.time - first_temperature.time);    //计算斜率
	for (int i = first_temperature.time; i < last_temperature.time; i += getPeriod())    //解压每个采集点的温度数据
	{
		double data = first_temperature.temperature + (i - first_temperature.time) * f;    //获取当前采集点温度
		oTemperatureData new_oTemperature;
		new_oTemperature.temperature = data;
		new_oTemperature.time = secondToTime(i);  //将数据转换为原始时间格式
		saveOTemperatureData(new_oTemperature);
	}
	return last_temperature;
}

humidityData uSDTHumidityData(humidityData first_humidity)    //解压缩湿度数据
{
	humidityData last_humidity = getHumidity();
	double f = (last_humidity.humidity - first_humidity.humidity) / (last_humidity.time - first_humidity.time);    //计算斜率
	for (int i = first_humidity.time; i < last_humidity.time; i += getPeriod())    //解压每个采集点的湿度数据
	{
		double data = first_humidity.humidity + (i - first_humidity.time) * f;    //获取当前采集点湿度
		oHumidityData new_oHumidity;
		new_oHumidity.humidity = data;
		new_oHumidity.time = secondToTime(i);  //将数据转换为原始时间格式
		saveOHumidityData(new_oHumidity);
	}
	return last_humidity;
}

void dataUncompress()    //解压缩过程
{
	temperatureData first_temperature = getTemperature();
	humidityData first_humidity = getHumidity();
	while (!isEmpty())
	{
		first_temperature = uSDTTemperatureData(first_temperature);
		first_humidity = uSDTHumidityData(first_humidity);
	}

}