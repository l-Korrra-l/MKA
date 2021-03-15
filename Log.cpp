#include "Log.h"
#include "LAN.h"
#include "Error.h"
#include <ctime>
#include "Parm.h"
#include <iostream>

namespace Log
{
	LOG getlog(wchar_t logfile[]) {
		LOG newLog{};
		newLog.stream = new std::ofstream;
		newLog.stream->open(logfile);			//L"protocol.txt"
		if (!newLog.stream->is_open())
			throw ERROR_THROW(112);
		wcscpy_s(newLog.logfile, logfile);
		return newLog;
	}

	void WriteLine(LOG newLog, char* c, ...) {
		char** pointer = &c;
		while (*pointer != "") {
			*newLog.stream << *pointer;
			pointer++;
		}
		*newLog.stream << std::endl;
	}

	void WriteLine(LOG newLog, wchar_t* c, ...) {
		wchar_t** pointer = &c;
		while (*pointer != L"") {
			char buffer[SIZE];
			size_t n = 0;
			wcstombs_s(&n, buffer, *pointer, SIZE);
			*newLog.stream << buffer;
			pointer++;
		}
		*newLog.stream << std::endl;
	}

	void WriteLog(LOG newLog) {
		time_t t = time(0);
		tm currentTime;
		localtime_s(&currentTime, &t);
		char str[TIME_SIZE];
		strftime(str, TIME_SIZE, "%d.%m.%Y %H:%M:%S", &currentTime);
		*newLog.stream << "--------- Протокол ---- Дата: " << str << "---------" << std::endl;
	}

	void WriteParm(LOG newLog, Parm::PARM parameter) {
		size_t n = 0;
		char buffer[PARM_MAX_SIZE];
		*newLog.stream << "--------- Параметры -------------------------------------" << std::endl;
		wcstombs_s(&n, buffer, parameter.log, PARM_MAX_SIZE);		*newLog.stream << "-log: " << buffer << std::endl;
		wcstombs_s(&n, buffer, parameter.out, PARM_MAX_SIZE);		*newLog.stream << "-out: " << buffer << std::endl;
		wcstombs_s(&n, buffer, parameter.in, PARM_MAX_SIZE);		*newLog.stream << "-in: " << buffer << std::endl;
	}

	void WriteIn(LOG newLog, In::IN in) {
		*newLog.stream << "--------- Исходные данные -------------------------------" << std::endl;
		*newLog.stream << "Количество символов: " << in.size << std::endl;
		*newLog.stream << "Проигнорировано    : " << in.ignor << std::endl;
		*newLog.stream << "Количество строк   : " << in.lines << std::endl;
	}

	void WriteError(LOG newLog, Error::ERROR newError) {
		if (newLog.stream == nullptr) {
			std::cout << "Ошибка " << newError.id << ": " << newError.message
				<< ", строка " << newError.inext.line << ", позиция " << newError.inext.col << std::endl;
		}
		else {
			*newLog.stream << "Ошибка " << newError.id << ": " << newError.message
				<< ", строка " << newError.inext.line << ", позиция " << newError.inext.col << std::endl;
		}
	}

	void Close(LOG newLog) {
		if (newLog.stream != nullptr) {
			newLog.stream->close();
			delete newLog.stream;
		}
	}
};

