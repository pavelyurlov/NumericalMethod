#pragma once

#include "minitypes.h"

const struct
{
	bool print_D = true; // печатать ли длинные выходные данные?
	bool one_kind = false;
	uint dimentions = 2; // не работает в случае одного вида
} preferences;

extern MatlabVector g_rh; // вектор координат точек
extern uint g_iterations_count; // количество работы, лог
extern num g_A; // размеры области
extern uint g_N; // количество точек на одной стороне
// наверное, стоит сделать весь input глобальным