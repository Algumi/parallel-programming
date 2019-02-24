#include <iostream>
#include <time.h>
#include <omp.h>

using namespace std;

void integral(const double a, const double b, const double h, double *res)
{
	long long i, n;
	double sum; // локальная переменная для подсчета интеграла
	double x; // координата точки сетки
	n = (long long)((b - a) / h); // количество точек сетки интегрирования
	sum = 0.0;
#pragma omp parallel for private(x) reduction(+: sum)
	for (i = 0; i < n; i++)
	{
		x = a + i * h + h / 2.0;
		sum += x / (x * x * x * x + 1) * h;
	}
	*res = sum;
}

// Число pi
#define PI 3.1415926535897932384626433832795
double experiment(double *res)
{
	double stime, ftime; // время начала и конца расчета
	double a = 0.0; // левая граница интегрирования
	double b = 10e5; // правая граница интегрирования
	double h = 0.001; // шаг интегрирования
	stime = clock();
	integral(a, b, h, res); // вызов функции интегрирования
	ftime = clock();
	return (ftime - stime) / CLOCKS_PER_SEC;
}

int main()
{
	int i; // переменная цикла
	double time; // время проведенного эксперимента
	double res; // значение вычисленного интеграла
	double min_time; // минимальное время работы
					 // реализации алгоритма
	double max_time; // максимальное время работы
					 // реализации алгоритма
	double avg_time; // среднее время работы
					 // реализации алгоритма
	int numbExp = 10; // количество запусков программы

	min_time = max_time = avg_time = experiment(&res);
	// оставшиеся запуски
	for (i = 0; i < numbExp - 1; i++)
	{
		time = experiment(&res);
		avg_time += time;
		if (max_time < time) max_time = time;
		if (min_time > time) min_time = time;
	}
	// вывод результатов эксперимента
	cout << "execution time : " << avg_time / numbExp << "; " <<
		min_time << "; " << max_time << endl;
	cout.precision(8);
	cout << "integral value : " << res << endl;
	return 0;
}