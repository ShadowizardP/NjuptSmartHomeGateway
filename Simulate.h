
#define N 5	//N个点
#define T 3 	//T次拟合
#define W 1	//权函数
#define PRECISION 0.00001

//int N = 5;	//N个点
float pow_n(float a, int n);
void mutiple(float a[][N], float b[][T + 1], float c[][T + 1]);    //矩阵相乘，得到法矩阵
void matrix_trans(float a[][T + 1], float b[][N]);    //矩阵转置
void init(float x_y[][2], float x[], float y[], int n);    //初始化组合数据点
void get_A(float matrix_A[][T + 1], float x_y[][2], int n);   //构建矩阵
void print_array(float array[][T + 1], int n);
void convert(float argu[][T + 2], int n);    //矩阵计算
void compute(float argu[][T + 2], int n, float root[]);    //计算拟合系数
void get_y(float trans_A[][N], float x_y[][2], float y[], int n);    //得到计算Y值
void cons_formula(float coef_A[][T + 1], float y[], float coef_form[][T + 2]);    //将Y值添加到矩阵
void simulateProcess(float x_point[], float y_point[], float result[]);    //拟合过程