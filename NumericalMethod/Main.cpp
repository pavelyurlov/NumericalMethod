// "solve_nonlinear_correct.m"

#include "IOSets.h"
#include "ERROR.h"
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
	is.d11 = 0.001;  is.d22 = 0.001; is.d12 = 0.001; is.d21 = 0.001;
	is.sw11 = 0.04, is.sw22 = 0.04, is.sw12 = 0.04, is.sw21 = 0.04;
	is.d1 = 0.2; is.d2 = 0.2;

	//----------Grid settings----------
	is.N = 2048;
	is.A = 1;

	//-------- - Numerical method Constants------ -
	is.max_iter = 500;

	//-------- - Closure Constants------ -
	is.a = 0.4;


	/*is.f_w11 = [](num r) {return normpdf_checked(r, sw11, 1); };
	is.f_w22 = [](num r) {return normpdf_checked(r, sw22, 1); };
	is.f_w12 = [](num r) {return normpdf_checked(r, sw12, 1); };
	is.f_w21 = [](num r) {return normpdf_checked(r, sw21, 1); };
	is.f_m1 = [](num r) {return normpdf_checked(r, sm1, 1); };
	is.f_m2 = [](num r) {return normpdf_checked(r, sm2, 1); };*/

	OutputSet os;
	os = solve_iter_sym(is);

	Json::Value result;
	result["Input"] = is;
	result["Output"] = os;

	std::cout << result;

	MPI_Finalize();
	return 0;
}
