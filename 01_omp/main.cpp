#include <iostream>
#include <time.h>
#include <omp.h>

using namespace std;

void integral(const double a, const double b, const double h, double *res)
{
	long long i, n;
	double sum; // ��������� ���������� ��� �������� ���������
	double x; // ���������� ����� �����
	n = (long long)((b - a) / h); // ���������� ����� ����� ��������������
	sum = 0.0;
#pragma omp parallel for private(x) reduction(+: sum)
	for (i = 0; i < n; i++)
	{
		x = a + i * h + h / 2.0;
		sum += x / (x * x * x * x + 1) * h;
	}
	*res = sum;
}

// ����� pi
#define PI 3.1415926535897932384626433832795
double experiment(double *res)
{
	double stime, ftime; // ����� ������ � ����� �������
	double a = 0.0; // ����� ������� ��������������
	double b = 10e5; // ������ ������� ��������������
	double h = 0.001; // ��� ��������������
	stime = clock();
	integral(a, b, h, res); // ����� ������� ��������������
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
	// ����� ����������� ������������
	cout << "execution time : " << avg_time / numbExp << "; " <<
		min_time << "; " << max_time << endl;
	cout.precision(8);
	cout << "integral value : " << res << endl;
	return 0;
}