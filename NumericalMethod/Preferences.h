#pragma once

#include "minitypes.h"

const struct
{
	bool print_D = false; // �������� �� ������� �������� ������?
	bool one_kind = false;
	uint dimentions = 3; // �� �������� � ������ ������ ����
} preferences;

extern uint g_iterations_count; // ���������� ������, ���
extern num g_A; // ������� �������
extern uint g_N; // ���������� ����� �� ����� �������
// ��������, ����� ������� ���� input ����������