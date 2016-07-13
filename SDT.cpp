
#include "support.h"
#include "SmartHome.h"

#define TE 0.5  //�¶���ת�ſ��
#define HE 5  //ʪ����ת�ſ��


oTemperatureData getOTemperature();	//��ȡԭʼ�¶�����
oHumidityData getOHumidity();	//��ȡԭʼʪ������
temperatureData getTemperature();	//��ȡ���¶�����
humidityData getHumidity();	//��ȡ��ʪ������
void saveTemperatureData(temperatureData first_temperature, temperatureData last_temperature);	//�洢ѹ������¶�����
void saveHumidityData(humidityData first_humidity, humidityData last_humidity);	//�洢ѹ�����ʪ������
temperatureData SDTTemperatureData(temperatureData first_temperature);    //��ת��ѹ���¶�����
humidityData SDTHumidityData(humidityData first_humidity);   //��ת��ѹ��ʪ������
void sendData();    //��������

int getPeriod();    //��ȡ���ݲɼ����
short isEmpty();    //�ж��Ƿ��ѹ���
void saveOTemperatureData(oTemperatureData data);    //�洢��ѹ������¶�����
void saveOHumidityData(oHumidityData data);    //�洢��ѹ�����ʪ��
temperatureData uSDTTemperatureData(temperatureData first_temperature);    //��ѹ���¶�����
humidityData uSDTHumidityData(humidityData first_humidity);    //��ѹ��ʪ������

oTemperatureData getOTemperature()	//��ȡԭʼ�¶�����
{
	oTemperatureData temperature = { { 2015, 12, 20, 16, 38, 0 }, 20.0 };

	return temperature;
}

oHumidityData getOHumidity()	//��ȡԭʼʪ������
{
	oHumidityData humidity = { { 2015, 12, 20, 16, 38, 0 }, 20.0 };

	return humidity;
}


temperatureData getTemperature()	//��ȡ���¶�����
{
	temperatureData temperature;
	oTemperatureData oTemperature = getOTemperature();
	temperature.temperature = oTemperature.temperature;
	temperature.time = timeToSecond(oTemperature.time);

	return temperature;
}

humidityData getHumidity()	//��ȡ��ʪ������
{
	humidityData humidity;
	oHumidityData oHumidity = getOHumidity();
	humidity.humidity = oHumidity.humidity;
	humidity.time = timeToSecond(oHumidity.time);

	return humidity;
}

void saveTemperatureData(temperatureData first_temperature, temperatureData last_temperature)	//�洢ѹ������¶�����
{

}

void saveHumidityData(humidityData first_humidity, humidityData last_humidity)	//�洢ѹ�����ʪ������
{

}

temperatureData SDTTemperatureData(temperatureData first_temperature)    //��ת��ѹ���¶�����
{
	temperatureData last_temperature = getTemperature();    //��ȡ��ʼ�յ�
	double gateTop = first_temperature.temperature + TE;    //������
	double gateBottom = first_temperature.temperature - TE;    //������
	double fTop = (last_temperature.temperature - gateTop) / (last_temperature.time - first_temperature.time);    //��ȡ��ʼ����б��
	double fBottom = (last_temperature.temperature - gateBottom) / (last_temperature.time - first_temperature.time);    //��ȡ��ʼ����б��
	while (1)
	{
		temperatureData new_temperature = getTemperature();    //��ȡ�½�������

		double fTopNew = (new_temperature.temperature - gateTop) / (new_temperature.time - first_temperature.time);    //����������б��
		double fBottomNew = (new_temperature.temperature - gateBottom) / (new_temperature.time - first_temperature.time);    //����������б��
		//�ж��Ƿ��������
		if (fTopNew > fTop)
		{
			fTop = fTopNew;
		}
		if (fBottomNew < fBottom)
		{
			fBottom = fBottomNew;
		}
		if (fTop >= fBottom)    //�ж����Ƿ��
		{
			//���򱾴�ѹ�����
			saveTemperatureData(first_temperature, last_temperature);
			return new_temperature;
		}
		else
		{
			//δ�������ѹ��
			last_temperature = new_temperature;
		}
	}
}



humidityData SDTHumidityData(humidityData first_humidity)    //��ת��ѹ��ʪ������
{
	humidityData last_humidity = getHumidity();    //��ȡ��ʼ�յ�
	double gateTop = first_humidity.humidity + HE;    //������
	double gateBottom = first_humidity.humidity - HE;    //������
	double fTop = (last_humidity.humidity - gateTop) / (last_humidity.time - first_humidity.time);    //��ȡ��ʼ����б��
	double fBottom = (last_humidity.humidity - gateBottom) / (last_humidity.time - first_humidity.time);    //��ȡ��ʼ����б��
	while (1)
	{
		humidityData new_humidity = getHumidity();    //��ȡ�½�������

		double fTopNew = (new_humidity.humidity - gateTop) / (new_humidity.time - first_humidity.time);    //����������б��
		double fBottomNew = (new_humidity.humidity - gateBottom) / (new_humidity.time - first_humidity.time);    //����������б��
		//�ж��Ƿ��������
		if (fTopNew > fTop)
		{
			fTop = fTopNew;
		}
		if (fBottomNew < fBottom)
		{
			fBottom = fBottomNew;
		}
		if (fTop >= fBottom)    //�ж����Ƿ��
		{
			//���򱾴�ѹ�����
			saveHumidityData(first_humidity, last_humidity);
			return new_humidity;
		}
		else
		{
			//δ�������ѹ��
			last_humidity = new_humidity;
		}
	}
}

void sendData()    //��������
{

}


void dataCompress()    //ѹ������
{
	//��ȡ��ʼ���
	temperatureData first_temperature = getTemperature();
	humidityData first_humidity = getHumidity();
	while (1)
	{
		if (isFull())    //���洢�ռ������������
		{
			sendData();
		}
		//����ѹ��
		first_temperature = SDTTemperatureData(first_temperature);
		first_humidity = SDTHumidityData(first_humidity);
	}
}

int getPeriod()    //��ȡ���ݲɼ����
{
	return 1800;
}

short isEmpty()    //�ж��Ƿ��ѹ���
{
	return 0;
}

void saveOTemperatureData(oTemperatureData data)    //�洢��ѹ������¶�����
{

}

void saveOHumidityData(oHumidityData data)    //�洢��ѹ�����ʪ��
{

}

temperatureData uSDTTemperatureData(temperatureData first_temperature)    //��ѹ���¶�����
{
	temperatureData last_temperature = getTemperature();
	double f = (last_temperature.temperature - first_temperature.temperature) / (last_temperature.time - first_temperature.time);    //����б��
	for (int i = first_temperature.time; i < last_temperature.time; i += getPeriod())    //��ѹÿ���ɼ�����¶�����
	{
		double data = first_temperature.temperature + (i - first_temperature.time) * f;    //��ȡ��ǰ�ɼ����¶�
		oTemperatureData new_oTemperature;
		new_oTemperature.temperature = data;
		new_oTemperature.time = secondToTime(i);  //������ת��Ϊԭʼʱ���ʽ
		saveOTemperatureData(new_oTemperature);
	}
	return last_temperature;
}

humidityData uSDTHumidityData(humidityData first_humidity)    //��ѹ��ʪ������
{
	humidityData last_humidity = getHumidity();
	double f = (last_humidity.humidity - first_humidity.humidity) / (last_humidity.time - first_humidity.time);    //����б��
	for (int i = first_humidity.time; i < last_humidity.time; i += getPeriod())    //��ѹÿ���ɼ����ʪ������
	{
		double data = first_humidity.humidity + (i - first_humidity.time) * f;    //��ȡ��ǰ�ɼ���ʪ��
		oHumidityData new_oHumidity;
		new_oHumidity.humidity = data;
		new_oHumidity.time = secondToTime(i);  //������ת��Ϊԭʼʱ���ʽ
		saveOHumidityData(new_oHumidity);
	}
	return last_humidity;
}

void dataUncompress()    //��ѹ������
{
	temperatureData first_temperature = getTemperature();
	humidityData first_humidity = getHumidity();
	while (!isEmpty())
	{
		first_temperature = uSDTTemperatureData(first_temperature);
		first_humidity = uSDTHumidityData(first_humidity);
	}

}