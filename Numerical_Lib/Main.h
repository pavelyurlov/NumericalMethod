#pragma once

/*!
@brief Главный класс, откуда всё запускается
@todo решить уже наконец, чем пользоваться, самописным или стандартным классом ошибок
*/
class Main
{
public:
	_declspec(dllimport) static int Count(int cmdn, char* cmd[]);
};