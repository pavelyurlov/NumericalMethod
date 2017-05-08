#pragma once

#include "macros.h"

/*!
@brief Главный класс, откуда всё запускается
@todo решить уже наконец, чем пользоваться, самописным или стандартным классом ошибок
*/
class Main
{
public:
	MSTESTPREFIX static int Count(int cmdn, char* cmd[]);
};