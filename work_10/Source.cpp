//MPI_Simpliest_c_Bind.cpp
#include "mpi.h"
#include <iostream>
#include "time.h"

using namespace std;

int NProc, ProcId;

void integral_simp(const double a, const double b, const double h, double *res)
{
	long long i, n;
	double sum1, sum2; // ��������� ���������� ��� �������� ���������
	double x; // ���������� ����� �����
	n = (long long)((b - a) / 2 / h); // ���������� ����� ����� ��������������
	sum1 = 0.0;
	sum2 = 0.0;

	int oper_num = n / NProc;
	for (i = oper_num * ProcId + 1; i <= oper_num * (ProcId + 1); i++)
	{
		x = a + (2 * i - 1) * h;
		sum1 += x / (x * x * x * x + 1);
	}

	for (i = oper_num * ProcId + 1; i < oper_num * (ProcId + 1); i++)
	{
		x = a + 2 * i * h;
		sum2 += x / (x * x * x * x + 1);
	}
	double res_sum1, res_sum2;
	MPI_Reduce(&sum1, &res_sum1, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&sum2, &res_sum2, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	*res = (h / 3) * (a / (a * a * a * a + 1) + (b / (b * b * b * b + 1)) + 4 * res_sum1 + 2 * res_sum2);

}

// integral value : 0.7853982
void integral(const double a, const double b, const double h, double *res)
{
	long long i, n;
	double sum; // ��������� ���������� ��� �������� ���������
	double x; // ���������� ����� �����
	n = (long long)((b - a) / h); // ���������� ����� ����� ��������������
	sum = 0.0;
	double res_buf;
	
	int oper_num = n / NProc;
	for (i = oper_num * ProcId; i < oper_num * (ProcId + 1); i++)
	{
		x = a + i * h + h / 2.0;
		sum += x / (x * x * x * x + 1) * h;
	}
	
	MPI_Reduce(&sum, res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
}

// ����� pi
#define PI 3.1415926535897932384626433832795
double experiment(double *res)
{
	double stime, ftime; // ����� ������ � ����� �������
	double a = 0.0; // ����� ������� ��������������
	double b = 10e4; // ������ ������� ��������������
	double h = 0.001; // ��� ��������������
	stime = clock();
	integral_simp(a, b, h, res); // ����� ������� ��������������
	ftime = clock();
	return (ftime - stime) / CLOCKS_PER_SEC;
}

int main()
{
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &NProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcId);
	int i; // ���������� �����
	double time; // ����� ������������ ������������
	double res; // �������� ������������ ���������
	double min_time; // ����������� ����� ������
					 // ���������� ���������
	double max_time; // ������������ ����� ������
					 // ���������� ���������
	double avg_time; // ������� ����� ������
					 // ���������� ���������
	int numbExp = 10; // ���������� �������� ���������

	min_time = max_time = avg_time = experiment(&res);
	// ���������� �������
	for (i = 0; i < numbExp - 1; i++)
	{
		time = experiment(&res);
		avg_time += time;
		if (max_time < time) max_time = time;
		if (min_time > time) min_time = time;
	}
	
	if (ProcId == 0) {
		// ����� ����������� ������������
		cout << "execution time : " << avg_time / numbExp << "; " <<
			min_time << "; " << max_time << endl;
		cout.precision(8);
		cout << "integral value : " << res << endl;
	}

	MPI_Finalize();
	return 0;
}