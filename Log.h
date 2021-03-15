#pragma once
#include <fstream>
#include "In.h"
#include "Error.h"
#include "Parm.h"
#define TIME_SIZE 25

namespace Log // ������ � ����������
{
	struct LOG	// ��������
	{
		wchar_t logfile[PARM_MAX_SIZE]; // ��� ����� ���������
		std::ofstream* stream;			// �������� ����� ���������
	};

	static const LOG INITLOG = { L"", NULL };		//��������� ��� ��������� ������������� LOG
	LOG getlog(wchar_t logfile[]);					//������������ ��������� LOG
	void WriteLine(LOG log, char* c, ...);			// ������� � �������� ������������ ��
	void WriteLine(LOG log, wchar_t* c, ...);		// ������� � �������� ������������ �����
	void WriteLog(LOG log);							//������� � �������� ���������
	void WriteParm(LOG log, Parm::PARM parm);		// ������� � ������� ���������� � ������� ����������
	void WriteIn(LOG log, In::IN in);				//������� � �������� ���������� � ������� ������
	void WriteError(LOG log, Error::ERROR error);	// ������� � �������� ��������� �� ������
	void WriteLexTable(LOG log);
	void WriteIdTable(LOG log);
	void Close(LOG log);							// ������� ��������
};