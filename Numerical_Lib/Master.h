#pragma once
// TODO: почистить input
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <mpi.h>
#include <chrono>
#include "./json/json.h"
#include "IOSets.h"
#include "minitypes.h"

/*!
@brief Подсчёт массива численных методов
@details Класс, который считает численный метод для каждого из входных InputSet и печатает результат в файл.
@todo всё в static?
*/
class Master
{
public:
	void Count(std::vector<InputSet> input_array);
private:
	uint m_iterations_count;
};



/*
extern OutputSet solve_iter_sym(InputSet);

uint g_iterations_count;

// печатает в файл
void count_input(std::vector<InputSet> input_array)
{
	int mpi_rank, mpi_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

	Json::Value root;
	root = Json::arrayValue;

	g_iterations_count = 0;
	std::chrono::high_resolution_clock::time_point time_point_start = std::chrono::high_resolution_clock::now();

	for (uint i = 0; i < input_array.size(); i++)
	{
		if (i % mpi_size == mpi_rank)
		{
			Json::Value newborn = Json::objectValue;
			newborn["JobNumber"] = i;
			newborn["Input"] = input_array[i];
			newborn["Output"] = solve_iter_sym(input_array[i]);
			root.append(newborn);
			newborn.clear(); // too paranoic?

			std::cout << "pr#" << mpi_rank << ": " << (float)i / input_array.size() << std::endl;
		}
	}

	std::chrono::high_resolution_clock::time_point time_point_finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>> (time_point_finish - time_point_start);

	std::ostringstream filename;
	filename << "proc_" << mpi_rank << "_" << mpi_size << ".txt";

	std::ofstream ofs;
	ofs.open(filename.str());

	ofs << root;
	ofs.close();

	std::cout << "proc#" << mpi_rank << "; iterations:" << g_iterations_count << "; time:" << time_span.count() << " sec; timePerIteration:" << time_span.count() / g_iterations_count << " sec";
}

*/