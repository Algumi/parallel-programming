#include <iostream>
#include <time.h>	
#include <math.h>
#include <omp.h>

using namespace std;

// ����� pi
#define PI 3.1415926535897932384626433832795

void integral(const double a1, const double b1, const double a2, const double b2, const double h1, const double h2, double *res)
{
	long long i, j, n, m;
	double sum1, sum2; // ��������� ���������� ��� �������� ���������
	double x, y; // ���������� ����� �����
	n = (long long)((b1 - a1) / h1); // ���������� ����� ����� �������������� �� X
	m = (long long)((b2 - a2) / h2); // ���������� ����� ����� �������������� �� Y
	sum1 = 0.0;
	sum2 = 0.0;
	double precalc = (b1 - a1) * (b2 - a2);
	omp_set_nested(1);
#pragma omp parallel for private(x, y, sum2) reduction(+: sum1)
	for (i = 0; i < n; i++)
	{
		sum2 = 0;
#pragma omp parallel for private(x, y) reduction(+: sum2)
		for (j = 0; j < m; j++)
		{
			x = a1 + i * h1 + h1 / 2;
			y = a2 + j * h2 + h2 / 2;
			sum2 += h1 * h2 * (exp(sin(PI * x) * cos(PI * y)) + 1) / precalc;
		}
		sum1 += sum2;
		sum2 = 0;
	}
	*res = sum1;
}

double experiment(double *res)
{
	double stime, ftime; // ����� ������ � ����� �������
	double a1 = 0.0; // ����� ������� �������������� �������
	double b1 = 16.0; // ������ ������� ��������������
	double a2 = 0.0; // ����� ������� ��������������
	double b2 = 16.0; // ������ ������� ��������������
	double h1 = 0.001; // ��� ��������������
	double h2 = 0.001; // ��� ��������������
	stime = clock();
	integral(a1, b1, a2, b2, h1, h2, res); // ����� ������� ��������������
	ftime = clock();
	return (ftime - stime) / CLOCKS_PER_SEC;
}

int main()
{
	int i; // ���������� �����
	double time; // ����� ������������ ������������
	double res; // �������� ������������ ���������
	double min_time; // ����������� ����� ������
					 // ���������� ���������
	double max_time; // ������������ ����� ������
					 // ���������� ���������
	double avg_time; // ������� ����� ������
					 // ���������� ���������
	int numbExp = 5; // ���������� �������� ���������

	min_time = max_time = avg_time = experiment(&res);
	// ���������� �������
	for (i = 0; i < numbExp - 1; i++)
	{
		time = experiment(&res);
		avg_time += time;
		if (max_time < time) max_time = time;
		if (min_time > time) min_time = time;
	}
	// ����� ����������� ������������
	cout << "execution time : " << avg_time / numbExp << "; " <<
		min_time << "; " << max_time << endl;
	cout.precision(8);
	cout << "integral value : " << res << endl;
	cin >> i;
	return 0;
}