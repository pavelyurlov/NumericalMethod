#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <mpi.h>
#include "./json/json.h"
#include "IOSets.h"
#include "minitypes.h"


extern OutputSet solve_iter_sym(InputSet);

// печатает в файл
void count_input(std::vector<InputSet> input_array)
{
	int mpi_rank, mpi_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

	std::ostringstream filename;
	filename << "proc_" << mpi_rank << "_" << mpi_size << ".txt";


	std::ofstream ofs;
	ofs.open(filename.str());

	for (uint i = 0; i < input_array.size(); i++)
	{
		if (i % mpi_size == mpi_rank)
		{
			Json::Value root;
			root["JobNumber"] = i;
			root["Input"] = input_array[i];
			root["Output"] = solve_iter_sym(input_array[i]);
			ofs << root;
			root.clear();

			/*ofs << "Job #" << i << " with input:" << std::endl;
			ofs << input_array[i] << std::endl;
			ofs << "output:" << std::endl;
			ofs << solve_iter_sym(input_array[i]);
			ofs << "end"<< std::endl;*/
		}
	}

	ofs.close();
}