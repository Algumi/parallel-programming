//MPI_Simpliest_c_Bind.cpp
#include"mpi.h"
#include<iostream>

using namespace std;

int main()
{
	int NProc, ProcId;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &NProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcId);
	cout <<"Total number of processes: "<<NProc<<"  Process identifier: "<<ProcId <<endl;
	MPI_Finalize();
	return 0;
}