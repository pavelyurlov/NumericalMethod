#pragma once

#include "minitypes.h"

const struct
{
	bool print_D = false; // �������� �� ������� �������� ������?
	bool one_kind = false;
	uint dimentions = 1; // �� �������� � ������ ������ ����
} preferences;

extern MatlabVector g_rh; // ������ ��������� �����
extern uint g_iterations_count; // ���������� ������, ���
extern num g_A; // ������� �������
extern uint g_N; // ���������� ����� �� ����� �������
// ��������, ����� ������� ���� input ����������