#pragma once

#include "minitypes.h"

struct
{
	bool print_D; // �������� �� ������� �������� ������?
	bool one_kind;
	uint dimentions; // �� �������� � ������ ������ ����
	uint max_iter;
} preferences;

extern MatlabVector g_rh; // ������ ��������� �����
extern uint g_iterations_count; // ���������� ������, ���
extern num g_A; // ������� �������
extern uint g_N; // ���������� ����� �� ����� �������
// ��������, ����� ������� ���� input ����������