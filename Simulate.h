
#define N 5	//N����
#define T 3 	//T�����
#define W 1	//Ȩ����
#define PRECISION 0.00001

//int N = 5;	//N����
float pow_n(float a, int n);
void mutiple(float a[][N], float b[][T + 1], float c[][T + 1]);    //������ˣ��õ�������
void matrix_trans(float a[][T + 1], float b[][N]);    //����ת��
void init(float x_y[][2], float x[], float y[], int n);    //��ʼ��������ݵ�
void get_A(float matrix_A[][T + 1], float x_y[][2], int n);   //��������
void print_array(float array[][T + 1], int n);
void convert(float argu[][T + 2], int n);    //�������
void compute(float argu[][T + 2], int n, float root[]);    //�������ϵ��
void get_y(float trans_A[][N], float x_y[][2], float y[], int n);    //�õ�����Yֵ
void cons_formula(float coef_A[][T + 1], float y[], float coef_form[][T + 2]);    //��Yֵ��ӵ�����
void simulateProcess(float x_point[], float y_point[], float result[]);    //��Ϲ���