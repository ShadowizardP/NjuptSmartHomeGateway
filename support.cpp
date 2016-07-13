#include "support.h"
#include <sstream>
#include <stdio.h>  
#include <stdlib.h>  
#include <string>  
#include <errno.h> 

oTemperatureData OriginalTemperatureData[10];
oHumidityData OriginalHumidityData[10];
THData TemperatureHumidityData[10];
cluster THClusterData[12];
int isSnaping = false;

/*
int CheckDir(char[] Dir)  
{  
    FILE *fp = NULL;  
    char TempDir[200];  
    memset(TempDir,'\0',sizeof(TempDir));  
    sprintf(TempDir,Dir);  
    strcat(TempDir,"\\");  
    strcat(TempDir,".temp.fortest");  
    fp = fopen(TempDir,"w");  
    if (!fp)  
    {  
        if(_mkdir(Dir)==0)  
        {  
            return 1;  
        }  
        else  
        {  
            return -1;//can not make a dir;  
        }  
    }  
    else  
    {  
        fclose(fp);  
    }  
    return 0;  
}
*/

int mkpath(std::string s,mode_t mode)  
{  
    size_t pre=0,pos;  
    std::string dir;  
    int mdret;  
  
    if(s[s.size()-1]!='/'){  
        // force trailing / so we can handle everything in loop  
        s+='/';  
    }  
  
    while((pos=s.find_first_of('/',pre))!=std::string::npos){  
        dir=s.substr(0,pos++);  
        pre=pos;  
        if(dir.size()==0) continue; // if leading / first time is 0 length  
        if((mdret=::mkdir(dir.c_str(),mode)) && errno!=EEXIST){  
            return mdret;  
        }  
    }  
    return mdret;  
}  

short isRunNian(short year)    //判断闰年    
{
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

short getFirstYear()    //获取初始年份
{
	return 2010;
}

int timeToSecond(zigbeeTime ot)    //计算初始年份开始的第n秒
{
	short oy = getFirstYear();
	int sec = 0;
	for (int i = oy; i < ot.year; i++)    //已过年份
	{
		if (isRunNian(i))
		{
			sec += 366 * 24 * 60 * 60;
		}
		else
		{
			sec += 365 * 24 * 60 * 60;
		}
	}

	short day;   //当年已过天数
	switch (ot.month)
	{
	case 1:
		day = ot.day - 1;
		break;
	case 2:
		day = 31 + ot.day - 1;
		break;
	case 3:
		day = 31 + (isRunNian(ot.year) ? 29 : 28) + ot.day - 1;
		break;
	case 4:
		day = 31 + (isRunNian(ot.year) ? 29 : 28) + 31 + ot.day - 1;
		break;
	case 5:
		day = 31 + (isRunNian(ot.year) ? 29 : 28) + 31 + 30 + ot.day - 1;
		break;
	case 6:
		day = 31 + (isRunNian(ot.year) ? 29 : 28) + 31 + 30 + 31 + ot.day - 1;
		break;
	case 7:
		day = 31 + (isRunNian(ot.year) ? 29 : 28) + 31 + 30 + 31 + 30 + ot.day - 1;
		break;
	case 8:
		day = 31 + (isRunNian(ot.year) ? 29 : 28) + 31 + 30 + 31 + 30 + 31 + ot.day - 1;
		break;
	case 9:
		day = 31 + (isRunNian(ot.year) ? 29 : 28) + 31 + 30 + 31 + 30 + 31 + 31 + ot.day - 1;
		break;
	case 10:
		day = 31 + (isRunNian(ot.year) ? 29 : 28) + 31 + 30 + 31 + 30 + 31 + 31 + 30 + ot.day - 1;
		break;
	case 11:
		day = 31 + (isRunNian(ot.year) ? 29 : 28) + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + ot.day - 1;
		break;
	case 12:
		day = 31 + (isRunNian(ot.year) ? 29 : 28) + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + ot.day - 1;
		break;
	default:
		break;
	}
	sec += day * 24 * 60 * 60;

	sec += ot.hour * 60 * 60 + ot.minute * 60 + ot.second;    //当天已过秒数

	return sec;
}

short isFull()    //判断存储空间是否紧张
{
	return 0;
}

short daysInMonth(short month, short year)        //获取每月天数
{
	short days;
	switch (month)
	{
	case 1:
		days = 31;
		break;
	case 2:
		if (isRunNian(year))
		{
			days = 29;
		}
		else{
			days = 28;
		}
		break;
	case 3:
		days = 31;
		break;
	case 4:
		days = 30;
		break;
	case 5:
		days = 31;
		break;
	case 6:
		days = 30;
		break;
	case 7:
		days = 31;
		break;
	case 8:
		days = 31;
		break;
	case 9:
		days = 30;
		break;
	case 10:
		days = 31;
		break;
	case 11:
		days = 30;
		break;
	case 12:
		days = 31;
		break;
	default:
		break;
	}
	return days;
}

zigbeeTime  secondToTime(int t)    //将初始年份开始第n秒恢复为正常时间格式
{
	zigbeeTime zigbeeT;
	short oy = getFirstYear();
	int days = t / (3600 * 24);    //已过天数
	int seconds = t % (3600 * 24);    //当天已过秒数
	short year = oy;
	while (days - (isRunNian(year) ? 366 : 365) >= 0)
	{
		days = days - (isRunNian(year) ? 366 : 365);
		year++;
	}
	zigbeeT.year = year;

	short month = 1;
	while (days - daysInMonth(month, year) >= 0)
	{
		days = days - daysInMonth(month, year);
		month++;
	}
	zigbeeT.month = month;
	zigbeeT.day = days + 1;

	short hour = 0;
	while (seconds - 3600 >= 0)
	{
		hour++;
		seconds -= 3600;
	}
	zigbeeT.hour = hour;

	short minute = 0;
	while (seconds - 60 >= 0)
	{
		minute++;
		seconds -= 60;
	}
	zigbeeT.minute = minute;
	zigbeeT.second = seconds;
	return zigbeeT;
}


string zigbeeTimeToString(zigbeeTime time)
{
	string year;
	string month;
	string day;
	string hour;
	string minute;
	string second;

	stringstream yearStream;
	yearStream<<time.year;
	yearStream>>year;

	stringstream monthStream;
		if(time.month < 10)
		{
			monthStream<<0;
		}
		monthStream<<time.month;
	monthStream>>month;


	stringstream dayStream;
		if(time.day < 10)
		{
			dayStream<<0;
		}
		dayStream<<time.day;
	dayStream>>day;

	stringstream hourStream;
		if(time.hour < 10)
		{
			hourStream<<0;
		}
		hourStream<<time.hour;
	hourStream>>hour;

	stringstream minuteStream;
		if(time.minute < 10)
		{
			minuteStream<<0;
		}
		minuteStream<<time.minute;
	minuteStream>>minute;

	stringstream secondStream;
		if(time.second < 10)
		{
			secondStream<<0;
		}
		secondStream<<time.second;
	secondStream>>second;

	string timeStr = year + month + day + hour + minute + second;
	return timeStr;

}
