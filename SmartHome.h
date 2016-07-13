//#include "dataStructs.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

static bool isCameraWorking = false;

int testOutlierProgress();    //判断异常值过程
void dataCompress();    //压缩数据
void dataUncompress();    //解压缩过程
void getSimulateResult();    //获取拟合计算结果
void messageAnalysis(char message[]);
void useCameraPreSet(string num);
void takeCameraSnap(int mount, int interval, const char* filePath);
