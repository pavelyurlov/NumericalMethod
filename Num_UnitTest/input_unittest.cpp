#include "stdafx.h"
#include "CppUnitTest.h"
#include <string.h>
#include <fstream>

#include "..\Numerical_Lib\InputParser.h"
#include "..\Numerical_Lib\Error.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// TODO: переместить рабочую папку в TestFiles, а то пути слишком длинные

void Success() // Оно в принципе бесполезно. Тест останавливается только в конце или по ошибке.
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
	catch (Error e) // TODO: Добавить всем классам namespace
	{
		if (e.GetMessage() != excepted_exception_text)
		{
			Assert::Fail((std::wstring(L"Не тот текст исключения. Получено: ") + e.GetMessage() + L"Ожидалось: " + excepted_exception_text.c_str() + L"Тест: " + message).c_str());
		}
	}
	catch (...)
	{
		Assert::Fail(Concat(L"Не тот тип исключения. Тест: ", message).c_str());
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
			Assert::AreEqual(L"foobar", Concat(w1, w2).c_str(), L"Слияние двух wchar_t строк");
		}

		TEST_METHOD(ConcatWcharNoFunction)
		{
			const wchar_t * w1 = L"foo";
			const wchar_t * w2 = L"bar";
			std::wstring s(w1);
			s += std::wstring(w2);
			s.c_str();
			Assert::AreEqual(L"foobar", s.c_str(), L"Слияние двух wchar_t строк");
		}
		TEST_METHOD(AssertErrorThrow)
		{
			AssertError([] {throw Error(L"foo"); }, L"foo", L"Обработка бросания исключений");
		}

		TEST_METHOD(FindWorkingDir)
		{
			std::ofstream o = std::ofstream("file_to_find_working_dir.txt");
			o.close();
		}

		TEST_METHOD(StaticCast)
		{
			double value = 16.00000;
			Assert::AreEqual(static_cast<uint>(16), static_cast<uint>(value), L"static_cast из double в uint");
		}
	};

	TEST_CLASS(InputUnitTest)
	{
	public:
		//**GENERAL**********************************************//

		TEST_METHOD(NoFile)
		{
			AssertError([] { InputParser::Parse("test_guаrantee_nonexistant_file.orly"); }, ERROR_INPUT_PARSER_FILE_DOES_NOT_EXIST, L"Чтение из несуществующего файла.");
		}

		TEST_METHOD(CantReadVersion)
		{
			AssertError([] { InputParser::Parse("..\\Num_UnitTest\\TestFiles\\test_bad_version_file.json"); }, ERROR_INPUT_PARSER_JSON_CANT_READ_VERSION, L"Плохая версия файла.");
		}

		TEST_METHOD(UnknownVersion)
		{
			AssertError([] { InputParser::Parse("..\\Num_UnitTest\\TestFiles\\test_unknown_version_file.json"); }, ERROR_INPUT_PARSER_UNKNOWN_VERSION, L"Неизвестная версия файла.");
		}
		//**VERSION 1**********************************************//

		TEST_METHOD(NormalFile)
		{
			std::vector<InputSet> res = InputParser::Parse("..\\Num_UnitTest\\TestFiles\\test_normal_file.json");
			Assert::AreEqual((float)17, (float)res[0].A,    L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)16, (float)res[0].N,    L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)18, (float)res[0].a,    L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)12, (float)res[0].sw11, L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)13, (float)res[0].sw12, L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)14, (float)res[0].sw21, L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)15, (float)res[0].sw22, L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)10, (float)res[0].sm1,  L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)11, (float)res[0].sm2,  L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)4,  (float)res[0].b1,   L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)5,  (float)res[0].b2,   L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)2,  (float)res[0].d1,   L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)3,  (float)res[0].d2,   L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)6,  (float)res[0].d11,  L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)7,  (float)res[0].d12,  L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)8,  (float)res[0].d21,  L"Обычное чтение из файла", LINE_INFO());
			Assert::AreEqual((float)9,  (float)res[0].d22,  L"Обычное чтение из файла", LINE_INFO());
		}

		TEST_METHOD(LinkFile)
		{
			//Нормальная ссылка
			Assert::Fail(L"UNDONE");
		}

		TEST_METHOD(GarbageLinkFile)
		{
			//Ссылка - не единственное поле
			Assert::Fail(L"UNDONE");
		}

		TEST_METHOD(VoidLinkFile)
		{
			//Ссылка в пустоту
			Assert::Fail(L"UNDONE");
		}

		TEST_METHOD(BadLinkFile)
		{
			//Ссылка на плохой файл
			Assert::Fail(L"UNDONE");
		}

		TEST_METHOD(MissingFieldFile)
		{
			//Отсутствие поля
			Assert::Fail(L"UNDONE");
		}
	};
}