// "solve_nonlinear_correct.m"

#include "Preferences.h"
#include "IOSets.h"
#include "InputParser.h"
#include "Master.h"
#include <iostream>
#include <mpi.h>

extern OutputSet solve_iter_sym(InputSet);

uint Preferences::dimentions;
bool Preferences::print_D;
bool Preferences::one_kind;
uint Preferences::max_iter;


int main(int cmdn, char* cmd[])
{
	MPI_Init(&cmdn, &cmd);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	std::cout << "Process " << rank << ", total proc count: " << size << std::endl;

	try
	{
		std::vector<InputSet> input = InputParser::Parse("input.json");
		count_input(input);
	}
	catch (std::exception e)
	{
		std::cout << "Unhandled exception: " << e.what() << "\n";
	}


	MPI_Finalize();
	return 0;
}
