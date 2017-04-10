
#include <time.h>
#include <unistd.h>
#include "SmartHome.h"
#include "support.h"
#include <string>
#include <sstream>
#include <fstream>
#include "httpRequest.h"

void dealWithDataMessage(char message[]);
void dealWithEventMessage(char message[]);
void storeData(char message[]);
void infraredEvent(char message[]);
void smokeEvent(char message[]);
bool cameraSnap(char point, int mount, int interval);
void moveTemperatureData(unsigned short p);
void moveHumidityData(unsigned short p);
zigbeeTime getTimeWithPeriod(unsigned short p);
void writeFile(string data,string dir,string file);

int count = 0;

void messageAnalysis(char message[])
{
	//printf("%s\n",message);
	if (message[0] == '$')    //数据类消息
	{
	//printf("HERE\n");
		dealWithDataMessage(message);
	}
	else if (message[0] == '!')    //事件类消息
	{
		if (message[4] == '0')
		{
			dealWithEventMessage(message);
		}
	}
}

void dealWithDataMessage(char message[])
{
	int p = message[5]-'0';
	string point;
	stringstream streamp;
	streamp<<p;
	streamp>>point;

	storeData(message);
	if(message[1] == 't')
	{
		count ++;
		if(count >= 2)
		{
			if (!testOutlierProgress())
			{
				cameraSnap(message[5], 5, 300);
			}
		}
	}
	switch (message[1])
	{
	case 't':
	{
		string tem;
		string time;
		stringstream stream;
		stream<<OriginalTemperatureData[9].temperature;
		stream>>tem;

		time = zigbeeTimeToString(OriginalTemperatureData[9].time);
		string url = "http://192.168.1.133:8080/SmartHome/SaveTemperature?temperature=" + tem + "&time=\"" + time +"\"&point=" + point;
		cout<<url<<endl;
		const char *u = url.data();
		getUrl(u);
		string dir = "/home/smartp-lab/temp/temperature";
		string point;
		stringstream pointStream;
		pointStream<<message[5];
		pointStream>>point;
		string file = "/home/smartp-lab/temp/temperature/temperature+"+point+".txt";
		string data = tem+"+"+time;
		writeFile(data,dir,file);
	}
		break;
	case 'h':
	{
		string hum;
		string time;
		stringstream stream;
		stream<<OriginalHumidityData[9].humidity;
		stream>>hum;

		time = zigbeeTimeToString(OriginalHumidityData[9].time);
		string url = "http://192.168.1.133:8080/SmartHome/SaveHumidity?humidity=" + hum + "&time=\"" + time +"\"&point="+ point;
		cout<<url<<endl;
		const char *u = url.data();
		getUrl(u);
		string dir = "/home/smartp-lab/temp/humidity";
		string point;
		stringstream pointStream;
		pointStream<<message[5];
		pointStream>>point;
		string file = "/home/smartp-lab/temp/temperature/humidity+"+point+".txt";
		string data = hum+"+"+time;
		writeFile(data,dir,file);
		
	}
		break;
	default:
		break;
	}
}

void writeFile(string data,string dir,string file)
{
	char *fi = new char[255];
	int i;
	for(i = 0 ; i<=file.length();i++)
		fi[i] = file[i];
	fi[i] = '\0';
	mode_t mode=0755;
	int flag = mkpath(dir,mode);
	if(flag == 0 || flag == -1)
	{ 
		ofstream fout(fi);
		fout<<data;
	}
}

void dealWithEventMessage(char message[])
{
	switch (message[1])
	{
	case 'i':
		infraredEvent(message);
		break;

	case 's':
		smokeEvent(message);
		break;

	default:
		break;
	}
}

void storeData(char message[])
{
	switch (message[1])
	{
	case 't':
	{
				moveTemperatureData(1);
				short tem = 0;
				tem = (message[3] - '0') * 10 + (message[4] - '0');
				if (message[2] == 'n')
				{
					tem = -tem;
				}
				OriginalTemperatureData[9].temperature = tem;
				OriginalTemperatureData[9].time = getTimeWithPeriod(message[6] - '0');
	}
		break;
	case 'h':
	{
				moveHumidityData(1);
				short hum = 0;
				hum = (message[3] - '0') * 10 + (message[4] - '0');
				OriginalHumidityData[9].humidity = hum;
				OriginalHumidityData[9].time = getTimeWithPeriod(message[6] - '0');
	}
		break;
	default:
		break;
	}
}

void moveTemperatureData(unsigned short p)
{
	for (int i = 0; i < 10 - p; i++)
	{
		OriginalTemperatureData[i] = OriginalTemperatureData[i + p];
	}
}

void moveHumidityData(unsigned short p)
{
	for (int i = 0; i < 10 - p; i++)
	{
		OriginalHumidityData[i] = OriginalHumidityData[i + p];
	}
}

zigbeeTime getTimeWithPeriod(unsigned short p)
{
	time_t now;         //实例化time_t结构
	struct tm *timenow;         //实例化tm结构指针
	time(&now);
	//time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
	time_t timeBefore = now - p * PERIOD * 60;
	//p时间间隔之前的时间
	timenow = localtime(&timeBefore);
	//localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区)
	zigbeeTime time;
	time.second = (*timenow).tm_sec;
	time.minute = (*timenow).tm_min;
	time.hour = (*timenow).tm_hour;
	time.day = (*timenow).tm_mday;
	time.month = (*timenow).tm_mon + 1;
	time.year = (*timenow).tm_year + 1900;
	return time;
}

void infraredEvent(char message[])
{
	switch (message[2])
	{
	case '0':
		break;

	case '1':
	case '2':
	case '3':
		cameraSnap(message[3], 5, 500);
		break;

	case '4':
	case '5':
	case '6':
		cameraSnap(message[3], 10, 300);
		break;
	case '7':
	case '8':
	case '9':
		cameraSnap(message[3], 20, 300);
		break;

	default:
		break;
	}
}

void smokeEvent(char message[])
{
	switch (message[2])
	{
	case '0':
	case '1':
	case '2':
		break;

	case '3':
	case '4':
		cameraSnap(message[3], 5, 500);
		break;

	case '5':
	case '6':
		cameraSnap(message[3], 10, 300);
		break;
	case '7':
	case '8':
	case '9':
		cameraSnap(message[3], 20, 300);
		break;

	default:
		break;
	}

}

bool cameraSnap(char point, int mount, int interval)
{
	//cout<<"!@#$"<<endl;
	if (!isCameraWorking)
	{
		int s = point - '0';
		string num;
		isCameraWorking = true;
		stringstream stream;
		stream<<s;
		stream>>num;
		//cout<<num<<num<<num<<num<<endl;
		//string num = stream.str();
		useCameraPreSet(num);
		sleep(1);
		string path = "/home/smartp-lab/temp/snapshot/";
		string time = zigbeeTimeToString(getTimeWithPeriod(0));
		path += time;
		cout<<path<<endl;
		mode_t mode=0755;
		int flag = mkpath(path,mode);
		if(flag == 0 || flag == -1)
		{ 
			const char* filepath = path.c_str();
			takeCameraSnap(mount, interval, filepath);
		}
		isCameraWorking = false;
		uploadImage(path,mount,time);
		return true;
	}
	else
	{
		return false;
	}
}
