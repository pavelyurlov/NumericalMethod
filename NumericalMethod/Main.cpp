// "solve_nonlinear_correct.m"

#include "IOSets.h"
#include "Master.h"
#include <iostream>
#include <mpi.h>

extern OutputSet solve_iter_sym(InputSet);

int main(int cmdn, char* cmd[])
{
	MPI_Init(&cmdn, &cmd);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	std::cout << "Process " << rank << ", total proc count: " << size << std::endl;


	InputSet is;

	//---------- - Model Constants--------
	is.sm1 = 0.04, is.sm2 = 0.06;
	is.b1 = 0.4; is.b2 = 0.4;
	is.d11 = 0.01; is.d22 = 0.001; is.d12 = 0.001; is.d21 = 0.001;
	is.sw11 = 0.4, is.sw22 = 0.04, is.sw12 = 0.04, is.sw21 = 0.04;
	is.d1 = 0.2; is.d2 = 0.2;

	//----------Grid settings----------
	is.N = 2048;
	is.A = 1;

	//-------- - Numerical method Constants------ -
	is.max_iter = 500;

	//-------- - Closure Constants------ -
	is.a = 0.4;



	std::vector<InputSet> input;


	//for (num d12 = 0; d12 < 0.001; d12 += 0.001 * 0.1)
		//for (num sm2 = 0; sm2 < 0.16; sm2 += 0.16 * 0.1)
		{
			//is.d12 = d12;
			//is.sm2 = sm2;
			input.push_back(is);
		}


	count_input(input);

	MPI_Finalize();
	return 0;
}
