//#include "dataStructs.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

static bool isCameraWorking = false;

int testOutlierProgress();    //�ж��쳣ֵ����
void dataCompress();    //ѹ������
void dataUncompress();    //��ѹ������
void getSimulateResult();    //��ȡ��ϼ�����
void messageAnalysis(char message[]);
void useCameraPreSet(string num);
void takeCameraSnap(int mount, int interval, const char* filePath);
