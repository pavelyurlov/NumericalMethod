#pragma once

#include "minitypes.h"
#include "MatlabVector.h"

struct Preferences
{
	static bool print_D; // �������� �� ������� �������� ������?
	static bool one_kind;
	static uint dimentions; // �� �������� � ������ ������ ����
	static uint max_iter;
};

extern MatlabVector g_rh; // ������ ��������� �����
extern uint g_iterations_count; // ���������� ������, ���
extern num g_A; // ������� �������
extern uint g_N; // ���������� ����� �� ����� �������
// ��������, ����� ������� ���� input ����������