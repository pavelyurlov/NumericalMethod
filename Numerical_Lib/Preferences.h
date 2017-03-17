#pragma once

#include "minitypes.h"
#include "MatlabVector.h"

struct Preferences
{
	static bool print_D; // печатать ли длинные выходные данные?
	static bool one_kind;
	static uint dimentions; // не работает в случае одного вида
	static uint max_iter;
};

extern MatlabVector g_rh; // вектор координат точек
extern uint g_iterations_count; // количество работы, лог
extern num g_A; // размеры области
extern uint g_N; // количество точек на одной стороне
// наверное, стоит сделать весь input глобальным