#pragma once

#include "minitypes.h"

struct
{
	bool print_D; // печатать ли длинные выходные данные?
	bool one_kind;
	uint dimentions; // не работает в случае одного вида
	uint max_iter;
} preferences;

extern MatlabVector g_rh; // вектор координат точек
extern uint g_iterations_count; // количество работы, лог
extern num g_A; // размеры области
extern uint g_N; // количество точек на одной стороне
// наверное, стоит сделать весь input глобальным