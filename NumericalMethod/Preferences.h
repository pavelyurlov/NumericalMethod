#pragma once

#include "minitypes.h"

const struct
{
	bool print_D = true; // �������� �� ������� �������� ������?
	bool one_kind = false;
	uint dimentions = 2; // �� �������� � ������ ������ ����
} preferences;

extern MatlabVector g_rh; // ������ ��������� �����
extern uint g_iterations_count; // ���������� ������, ���
extern num g_A; // ������� �������
extern uint g_N; // ���������� ����� �� ����� �������
// ��������, ����� ������� ���� input ����������