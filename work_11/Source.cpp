//MPI_Simpliest_c_Bind.cpp
#include "mpi.h"
#include <iostream>
#include "time.h"

using namespace std;

int NProc, ProcId;

// Число pi
#define PI 3.1415926535897932384626433832795

void integral(const double a1, const double b1, const double a2, const double b2, const double h1, const double h2, double *res) 
{
	long long i, j, n, m;
	double sum1, sum2; // локальная переменная для подсчета интеграла
	double x, y; // координата точки сетки
	n = (long long)((b1 - a1) / h1); // количество точек сетки интегрирования по X
	m = (long long)((b2 - a2) / h2); // количество точек сетки интегрирования по Y
	sum1 = 0.0;
	sum2 = 0.0;
	double precalc = (b1 - a1) * (b2 - a2);
	int oper_num = n / NProc;
	for (i = oper_num * ProcId; i < oper_num * (ProcId + 1); i++)
	{
		sum2 = 0;
		for (j = 0; j < m; j++)
		{
			x = a1 + i * h1 + h1 / 2;
			y = a2 + j * h2 + h2 / 2;
			sum2 += h1 * h2 * (exp(sin(PI * x) * cos(PI * y)) + 1) / precalc;
		}
		sum1 += sum2;
	}

	MPI_Reduce(&sum1, res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
}

// Число pi
#define PI 3.1415926535897932384626433832795
double experiment(double *res)
{
	double stime, ftime; // время начала и конца расчета
	double a1 = 0.0; // левая граница интегрирования первого
	double b1 = 16.0; // правая граница интегрирования
	double a2 = 0.0; // левая граница интегрирования
	double b2 = 16.0; // правая граница интегрирования
	double h1 = 0.001; // шаг интегрирования
	double h2 = 0.001; // шаг интегрирования
	stime = clock();
	integral(a1, b1, a2, b2, h1, h2, res); // вызов функции интегрирования
	ftime = clock();
	return (ftime - stime) / CLOCKS_PER_SEC;
}

int main()
{
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &NProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcId);
	int i; // переменная цикла
	double time; // время проведенного эксперимента
	double res; // значение вычисленного интеграла
	double min_time; // минимальное время работы
					 // реализации алгоритма
	double max_time; // максимальное время работы
					 // реализации алгоритма
	double avg_time; // среднее время работы
					 // реализации алгоритма
	int numbExp = 1; // количество запусков программы

	min_time = max_time = avg_time = experiment(&res);
	// оставшиеся запуски
	for (i = 0; i < numbExp - 1; i++)
	{
		time = experiment(&res);
		avg_time += time;
		if (max_time < time) max_time = time;
		if (min_time > time) min_time = time;
	}

	if (ProcId == 0) {
		// вывод результатов эксперимента
		cout << "execution time : " << avg_time / numbExp << "; " <<
			min_time << "; " << max_time << endl;
		cout.precision(8);
		cout << "integral value : " << res << endl;
	}

	MPI_Finalize();
	return 0;
}