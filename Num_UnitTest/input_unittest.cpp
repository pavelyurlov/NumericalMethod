#include "stdafx.h"
#include "CppUnitTest.h"
#include <string.h>
#include <fstream>

#include "..\Numerical_Lib\InputParser.h"
#include "..\Numerical_Lib\Error.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// TODO: ����������� ������� ����� � TestFiles, � �� ���� ������� �������

void Success() // ��� � �������� ����������. ���� ��������������� ������ � ����� ��� �� ������.
{
	Assert::AreEqual(1, 1);
}

// TODO Smart wchar_t* pointers
std::wstring Concat(const wchar_t *ws1, const wchar_t *ws2)
{
	std::wstring s(ws1);
	s += std::wstring(ws2);
	return s;
}

void AssertError(void (*func)(), std::wstring excepted_exception_text, const wchar_t* message = NULL)
{
	try
	{
		func();
	}
	catch (Error e) // TODO: �������� ���� ������� namespace
	{
		if (e.GetMessage() != excepted_exception_text)
		{
			Assert::Fail((std::wstring(L"�� ��� ����� ����������. ��������: ") + e.GetMessage() + L"���������: " + excepted_exception_text.c_str() + L"����: " + message).c_str());
		}
	}
	catch (...)
	{
		Assert::Fail(Concat(L"�� ��� ��� ����������. ����: ", message).c_str());
	}
}

namespace Num_UnitTest
{		
	TEST_CLASS(PolygonUnitTest)
	{
	public:

		TEST_METHOD(ConcatWchar)
		{
			const wchar_t * w1 = L"foo";
			const wchar_t * w2 = L"bar";
			Assert::AreEqual(L"foobar", Concat(w1, w2).c_str(), L"������� ���� wchar_t �����");
		}

		TEST_METHOD(ConcatWcharNoFunction)
		{
			const wchar_t * w1 = L"foo";
			const wchar_t * w2 = L"bar";
			std::wstring s(w1);
			s += std::wstring(w2);
			s.c_str();
			Assert::AreEqual(L"foobar", s.c_str(), L"������� ���� wchar_t �����");
		}
		TEST_METHOD(AssertErrorThrow)
		{
			AssertError([] {throw Error(L"foo"); }, L"foo", L"��������� �������� ����������");
		}

		TEST_METHOD(FindWorkingDir)
		{
			std::ofstream o = std::ofstream("file_to_find_working_dir.txt");
			o.close();
		}
	};

	TEST_CLASS(InputUnitTest)
	{
	public:
		//**GENERAL**********************************************//

		TEST_METHOD(NoFile)
		{
			AssertError([] { InputParser::Parse("test_gu�rantee_nonexistant_file.orly"); }, ERROR_INPUT_PARSER_FILE_DOES_NOT_EXIST, L"������ �� ��������������� �����.");
		}

		TEST_METHOD(CantReadVersion)
		{
			AssertError([] { InputParser::Parse("..\\Num_UnitTest\\TestFiles\\test_bad_version_file.json"); }, ERROR_INPUT_PARSER_JSON_CANT_READ_VERSION, L"������ ������ �����.");
		}

		TEST_METHOD(UnknownVersion)
		{
			AssertError([] { InputParser::Parse("..\\Num_UnitTest\\TestFiles\\test_unknown_version_file.json"); }, ERROR_INPUT_PARSER_UNKNOWN_VERSION, L"����������� ������ �����.");
		}
		//**VERSION 1**********************************************//

		TEST_METHOD(NormalFile)
		{
			std::vector<InputSet> res = InputParser::Parse("..\\Num_UnitTest\\TestFiles\\test_normal_file.json");
			Assert::AreEqual((float)res[0].b1, (float)4, L"������� ������ �� �����", LINE_INFO());
			// TODO ��� ����
		}

		/*
		������ - �� ������������ ����
		���������� ������
		������ � �������
		������ �� ������ ����
		���������� ����
		*/

	};
}