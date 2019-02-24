//Reduct1.cpp
#include <stdio.h>
int main()
{
	int count = 0;
#pragma omp parallel reduction (+: count)
	{
		count++;
		printf("Current value of the count: %d\n", count);
	}
	printf("Threads number: %d\n", count);
	return 0;
}