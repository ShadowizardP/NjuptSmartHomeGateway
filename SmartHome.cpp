#include "SmartHome.h"
#include <stdio.h>
#include <unistd.h> /*Unix标准函数定义*/
#include <sys/types.h> /**/
#include <sys/stat.h> /**/
#include <fcntl.h> /*文件控制定义*/
#include <termios.h> /*PPSIX终端控制定义*/
#include <errno.h> /*错误号定义*/
#include <stdlib.h>
#include <string.h> 
#include <thread> 
//#include <pthread.h>
using namespace std;

//void * progress(void *arg);
//char buffer[10][100];
//int buf[10];
//int count = 0;

int main(int argc, char *argv[])
{
	int fd, readnum;
//	string d = "/dev/ttys0";
//	const char *dev = d.data();
	char *dev="/dev/ttyS0";
	char recv[100];
	char *p = recv;
	struct termios opt, oldopt;

	fd = open(dev, O_RDWR);

	tcgetattr(fd, &oldopt);

	tcgetattr(fd, &opt);
	cfsetispeed(&opt, B115200);//115200
	cfsetospeed(&opt, B115200);

	opt.c_cflag &= ~PARENB; //N
	opt.c_cflag &= ~INPCK;
	opt.c_cflag &= ~CSTOPB;//1
	opt.c_cflag &= ~CSIZE;
	opt.c_cflag |= CS8; //8

	opt.c_iflag &= ~(IXON | IXOFF | IXANY);

	opt.c_cc[VTIME] = 0;
	opt.c_cc[VMIN] = 0;

	opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /*Input*/
	opt.c_oflag &= ~OPOST; /*Output*/

	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &opt);

	//pipe(buf);

	while (1)
	{
		bzero(recv, 100);
		while (read(fd, recv, 8)>0)
		{
			//memcpy(buffer[count],recv,8);
			//write(buf[count],recv,8);
			printf("%s\n", recv);
			//pthread_t tid;
			//pthread_create(&tid,NULL,progress,&count);
			//count++;
			//if(count == 10)
			//{
			//	count = 0;
			//}
			std::thread t(messageAnalysis,recv);
			t.detach();
			sleep(1);
		}
	}

	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &oldopt);
	close(fd);
	return 0;
}
/*
void * progress(void *arg)
{
	int c = *(int *)arg;
	char message[100];
	read(buf[c],message,8);
	//memcpy(message,buffer[c],8);
	printf("@@@@@@@@@@@@@@%s\n", message);
	messageAnalysis(message);	
}
*/
