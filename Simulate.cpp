#include "Simulate.h"
#include <stdio.h>
//#include <conio.h>
#include <math.h>
//#include <process.h>
#include <pthread.h>
#include "SmartHome.h"

float pow_n(float a, int n)
{
	int i;
	if (n == 0)
		return(1);
	float res = a;
	for (i = 1; i<n; i++)
	{
		res *= a;
	}
	return(res);
}
void mutiple(float a[][N], float b[][T + 1], float c[][T + 1])    //矩阵相乘，得到法矩阵
{
	float res = 0;
	int i, j, k;
	for (i = 0; i<T + 1; i++)
	for (j = 0; j<T + 1; j++)
	{
		res = 0;
		for (k = 0; k<N; k++)
		{
			res += a[i][k] * b[k][j];
			c[i][j] = res;
		}
	}
}
void matrix_trans(float a[][T + 1], float b[][N])    //矩阵转置
{
	int i, j;
	for (i = 0; i<N; i++)
	{
		for (j = 0; j<T + 1; j++)
		{
			b[j][i] = a[i][j];
		}
	}
}
void init(float x_y[][2], float x[], float y[], int n)    //初始化组合数据点
{
	int i;

	for (i = 0; i<n; i++)
	{
		x_y[i][0] = x[i];
		x_y[i][1] = y[i];
	}
}
void get_A(float matrix_A[][T + 1], float x_y[][2], int n)    //构建矩阵
{
	int i, j;
	for (i = 0; i<N; i++)
	{
		for (j = 0; j<T + 1; j++)
		{
			matrix_A[i][j] = W*pow_n(x_y[i][0], j);
		}
	}
}
void print_array(float array[][T + 1], int n)
{
	int i, j;
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<T + 1; j++)
		{
			printf("%-g	", array[i][j]);
		}
		printf("\n");
	}
}
void convert(float argu[][T + 2], int n)    //矩阵计算
{
	int i, j, k, p, t;
	float rate, temp;
	for (i = 1; i<n; i++)
	{
		for (j = i; j<n; j++)
		{
			if (argu[i - 1][i - 1] == 0)
			{
				for (p = i; p<n; p++)
				{
					if (argu[p][i - 1] != 0)
						break;
				}
				if (p == n)
				{
					printf("方程组无解!\n");
					exit(0);
				}
				for (t = 0; t<n + 1; t++)
				{
					temp = argu[i - 1][t];
					argu[i - 1][t] = argu[p][t];
					argu[p][t] = temp;
				}
			}
			rate = argu[j][i - 1] / argu[i - 1][i - 1];
			for (k = i - 1; k<n + 1; k++)
			{
				argu[j][k] -= argu[i - 1][k] * rate;
				if (fabs(argu[j][k]) <= PRECISION)
					argu[j][k] = 0;
			}
		}
	}
}
void compute(float argu[][T + 2], int n, float root[])    //计算拟合系数
{
	int i, j;
	float temp;
	for (i = n - 1; i >= 0; i--)
	{
		temp = argu[i][n];
		for (j = n - 1; j>i; j--)
		{
			temp -= argu[i][j] * root[j];
		}
		root[i] = temp / argu[i][i];
	}
}
void get_y(float trans_A[][N], float x_y[][2], float y[], int n)    //得到计算Y值
{
	int i, j;
	float temp;
	for (i = 0; i<n; i++)
	{
		temp = 0;
		for (j = 0; j<N; j++)
		{
			temp += trans_A[i][j] * x_y[j][1];
		}
		y[i] = temp;
	}
}
void cons_formula(float coef_A[][T + 1], float y[], float coef_form[][T + 2])    //将Y值添加到矩阵
{
	int i, j;
	for (i = 0; i<T + 1; i++)
	{
		for (j = 0; j<T + 2; j++)
		{
			if (j == T + 1)
				coef_form[i][j] = y[i];
			else
				coef_form[i][j] = coef_A[i][j];
		}
	}
}
void print_root(float a[], int n)
{
	int i, j;
	printf("%d个点的%d次拟合的多项式系数为:\n", N, T);
	for (i = 0; i<n; i++)
	{
		printf("a[%d]=%g,	", i + 1, a[i]);
	}
	printf("\n");
	printf("拟合曲线方程为:\ny(x)=%g", a[0]);
	for (i = 1; i<n; i++)
	{
		printf(" + %g", a[i]);
		for (j = 0; j<i; j++)
		{
			printf("*X");
		}
	}
	printf("\n");
}
//float * process(float x_point[],float y_point[])
void simulateProcess(float x_point[], float y_point[], float result[])    //拟合过程
{
	float x_y[N][2], matrix_A[N][T + 1], trans_A[T + 1][N], coef_A[T + 1][T + 1], coef_formu[T + 1][T + 2], y[T + 1]; //, a[T + 1]
	init(x_y, x_point, y_point, N);    //初始化组合数据点
	get_A(matrix_A, x_y, N);    //构建矩阵
	//printf("矩阵a为：\n");
	//print_array(matrix_A,N);
	matrix_trans(matrix_A, trans_A);    //矩阵转置
	mutiple(trans_A, matrix_A, coef_A);    //矩阵相乘，得到法矩阵
	//printf("法矩阵为：\n");
	//print_array(coef_A,T+1);
	get_y(trans_A, x_y, y, T + 1);    //得到计算Y值
	cons_formula(coef_A, y, coef_formu);    //将Y值添加到矩阵
	convert(coef_formu, T + 1);    //矩阵计算
	compute(coef_formu, T + 1, result);    //计算拟合系数
	//compute(coef_formu	,	T+1	,	a);
	//*return(a);
}
