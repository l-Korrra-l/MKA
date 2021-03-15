#pragma once
#include <fstream>
#include "In.h"
#include "Error.h"
#include "Parm.h"

namespace Out 
{
	struct OUT	
	{
		wchar_t outfile[PARM_MAX_SIZE]; // имя файла 
		std::ofstream* stream;			// выходной поток 
	};

	static const OUT INITOUT = { L"", NULL };		//структура для начальной инициализации 
	OUT getout(wchar_t outfile[]);					//сформировать структуру
	void WriteOut(OUT newOut, In::IN in);
	void Close(OUT out);							// закрыть
};

