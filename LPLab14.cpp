#include <iostream>
#include <locale>
#include <cwchar>
#include <iomanip>

#include "Parm.h"
#include "Error.h"
#include "In.h"
#include "Log.h"
#include "Out.h"
#include "ASMGenerator.h"
#include "Polska.h"
#include "MFST.h"
#include "FST.h"
#include "LAN.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	Log::LOG log = Log::INITLOG;
	Out::OUT out = Out::INITOUT;
	try
	{
		std::cout << "_________________ Входные параметры ________________________________________________" << std::endl << std::endl;
		Parm::PARM parm = Parm::getparm(argc, argv);
		std::wcout << " -in: " << parm.in << "\n-out: " << parm.out << "\n-log: " << parm.log << std::endl << std::endl;

		log = Log::getlog(parm.log);
		Log::WriteLine(log, (char*)"Тест: ", "без ошибок ", "");

		out = Out::getout(parm.out);
		In::IN in = In::getin(parm.in);
		in.change();
		in.EditText();
		std::cout << "_________________ Получено из файла ________________________________________________\n\n";
		std::cout << "Всего символов: " << in.size << std::endl;
		std::cout << "Всего строк   : " << in.lines << std::endl;
		std::cout << "Пропущено     : " << in.ignor << std::endl;


		std::cout << "\n_________________ Преобразовано ____________________________________________________\n\n";
		std::cout << in.text;
		Out::WriteOut(out, in);
		Out::Close(out);

		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);


		Lan::LAN lan(log);
		for (int i = 0; i < strlen((char*)in.text); i++) {
			lan.InspectSymbol(in.text[i]);
		}
		
		lan.WriteIDTable();
		std::cout << "\n____________________________________________________________________________________\n\n";
	

		MFST_TRACE_START //отладка
		MFST::Mfst synt = MFST::Mfst::Mfst(lan, GRB::getGreibach()); //автомат
		synt.start();				//старт синтаксического анализа
		synt.savededucation();
		synt.printrules();
		PN polska;
		if (polska.searchExpression(lan))
		{
			std::cout << std::endl << "Польская запись построена!" << std::endl;

		}
		else {
			std::cout << std::endl << "Польская запись не построена!" << std::endl;

		}
		lan.Print();
		polska.Print(lan);
		Log::Close(log);
		ASMGenerator::Generate(lan);
	}

	catch (Error::ERROR e)
	{
		if (e.inext.line == -1 || e.inext.col == -1)
			std::cout << "Ошибка " << e.id << ": " << e.message << std::endl << std::endl;
		else {
			std::cout << "Ошибка " << e.id << ": " << e.message << std::endl;
			std::cout << "строка " << e.inext.line << " позиция " << e.inext.col << std::endl << std::endl;
		}
		//Log::WriteError(log, e);
	};
	system("pause");
	return 0;
}
