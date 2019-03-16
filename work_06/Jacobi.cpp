#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include "omp.h"

int* pPivotPos; // The number of pivot rows selected at the iterations
int* pPivotIter; // The iterations, at which the rows were pivots
const double eps = 0.001; // accuracy

typedef struct {
	int PivotRow;
	double MaxValue;
} TThreadPivotRow;

void Jacobi(int Size, double* pMatrix, double* pVector, double* X)
{
	double* TempX = new double[Size];
	double norm; // норма, определяемая как наибольшая разность компонент столбца иксов соседних итераций.

	do {
#pragma omp parallel for
		for (int i = 0; i < Size; i++) {
			TempX[i] = pVector[i];
			for (int g = 0; g < Size; g++) {
				if (i != g)
					TempX[i] -= pMatrix[i * Size + g] * X[g];
			}
			TempX[i] /= pMatrix[i * Size + i];
		}
		norm = fabs(X[0] - TempX[0]);
		for (int h = 0; h < Size; h++) {
			if (fabs(X[h] - TempX[h]) > norm)
				norm = fabs(X[h] - TempX[h]);
			X[h] = TempX[h];
		}
	} while (norm > eps);
	delete[] TempX;
}

void DummyDataInitialization(double* pMatrix, double* pVector, int Size) {
	int i, j; // Loop variables
	for (i = 0; i<Size; i++) {
		pVector[i] = i + 1;
		for (j = 0; j<Size; j++) {
			if (j <= i)
				pMatrix[i*Size + j] = 1;
			else
				pMatrix[i*Size + j] = 0;
		}
	}
}

// Function for random initialization of the matrix
// and the vector elements
void RandomDataInitialization(double* pMatrix, double* pVector, int Size) {
	int i, j; // Loop variables
	srand(unsigned(clock()));
	for (i = 0; i<Size; i++) {
		pVector[i] = rand() / double(1000);
		for (j = 0; j<Size; j++) {
			if (j <= i)
				pMatrix[i*Size + j] = rand() / double(1000);
			else
				pMatrix[i*Size + j] = 0;
		}
	}
}

void MyDataInitialization(double* &pMatrix, double* &pVector, int Size) {
	pVector = new double[Size] { 14, 18, 32, 16, 11 };
	pMatrix = new double[Size * Size]
	{ 6, 6, 5, 18, 20,
		10, 9, 7, 24, 30,
		12, 12, 13, 27, 35,
		8, 6, 6, 15, 20,
		4, 5, 4, 15, 15 };
}

// Function for memory allocation and definition of the objects elements 
void ProcessInitialization(double* &pMatrix, double* &pVector, double* &pResult, int &Size) {
	// Setting the size of the matrix and the vector
	do {
		printf("\nEnter size of the matrix and the vector: ");
		scanf_s("%d", &Size);
		printf("\nChosen size = %d \n", Size);
		if (Size <= 0)
			printf("\nSize of objects must be greater than 0!\n");
	} while (Size <= 0);
	// Memory allocation
	pMatrix = new double[Size*Size];
	pVector = new double[Size];
	pResult = new double[Size];
	// Initialization of the matrix and the vector elements
	for (int i = 0; i < Size; i++)
		pResult[i] = 0;

	// For work_04
	DummyDataInitialization(pMatrix, pVector, Size);

	// For work_05
	//MyDataInitialization(pMatrix, pVector, Size);
	//RandomDataInitialization(pMatrix, pVector, Size);
}

// Function for formatted matrix output
void PrintMatrix(double* pMatrix, int RowCount, int ColCount) {
	int i, j; // Loop variables
	for (i = 0; i<RowCount; i++) {
		for (j = 0; j<ColCount; j++)
			printf("%7.4f ", pMatrix[i*RowCount + j]);
		printf("\n");
	}
}

// Function for formatted vector output
void PrintVector(double* pVector, int Size) {
	int i;
	for (i = 0; i<Size; i++)
		printf("%7.4f ", pVector[i]);
}

// Function for computational process termination
void ProcessTermination(double* pMatrix, double* pVector, double*
	pResult) {
	delete[] pMatrix;
	delete[] pVector;
	delete[] pResult;
}

// Function for testing the result
void TestResult(double* pMatrix, double* pVector,
	double* pResult, int Size) {
	/* Buffer for storing the vector, that is a result of multiplication
	of the linear system matrix by the vector of unknowns */
	double* pRightPartVector;
	// Flag, that shows wheather the right parts
	// vectors are identical or not
	int equal = 0;
	double Accuracy = 1.e-6; // Comparison accuracy
	pRightPartVector = new double[Size];
	for (int i = 0; i<Size; i++) {
		pRightPartVector[i] = 0;
		for (int j = 0; j<Size; j++) {
			pRightPartVector[i] +=
				pMatrix[i*Size + j] * pResult[j];
		}
	}
	for (int i = 0; i<Size; i++) {
		if (fabs(pRightPartVector[i] - pVector[i]) > Accuracy)
			equal = 1;
	}
	if (equal == 1)
		printf("\nThe result of the parallel Gauss algorithm is NOT correct."
			"Check your code.");
	else
		printf("\nThe result of the Jacobi algorithm is correct.");
	delete[] pRightPartVector;
}

int main() {
	double* pMatrix; // The matrix of the linear system
	double* pVector; // The right parts of the linear system
	double* pResult; // The result vector
	int Size; // The size of the matrix and the vectors
	double start, finish, duration;
	// Data initialization
	ProcessInitialization(pMatrix, pVector, pResult, Size);
	// The matrix and the vector output

	//printf("Initial Matrix \n");
	//PrintMatrix(pMatrix, Size, Size);
	//printf("Initial Vector \n");
	//PrintVector(pVector, Size);


	start = omp_get_wtime();
	Jacobi(Size, pMatrix, pVector, pResult);
	finish = omp_get_wtime();
	duration = finish - start;

	// The matrix and the vector output
	/*
	printf("\nRecalculated Matrix \n");
	PrintMatrix(pMatrix, Size, Size);
	printf("Recalculated Vector \n");
	PrintVector(pVector, Size);
	*/
	//printf("\nResult Vector \n");
	//PrintVector(pResult, Size);

	// Testing the result
	TestResult(pMatrix, pVector, pResult, Size);
	// Printing the time spent by parallel Gauss algorithm
	printf("\n Time of execution: %f\n", duration);


	// Program termination
	ProcessTermination(pMatrix, pVector, pResult);
	system("pause");
	return 0;
}
