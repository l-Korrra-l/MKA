#pragma once
#include <tchar.h>
#define PARM_IN L"-in:"			// ���� ��� ����� ��������� ����
#define PARM_OUT L"-out:"		// ���� ��� ����� ���������� ����
#define PARM_LOG L"-log:"		// ���� ��� ����� �������
#define PARM_MAX_SIZE 300		// ������������ ������ ������ ���������
#define PARAM_OUT_DEFAULT_EXT L".out" //���������� ����� ���������� ���� �� ���������
#define PARM_LOG_DEFAULT_EXC  L".log" // ���������� ����� ��������� �� ���������

namespace Parm // ��������� ������� ����������
{
	struct PARM	//������� ���������
	{
		wchar_t in[PARM_MAX_SIZE];		// -in: ��� ����� ��������� ����
		wchar_t out[PARM_MAX_SIZE];		// -out: ��� ����� ��������� ����
		wchar_t log[PARM_MAX_SIZE];		// -log: ��� ����� ��������� ����
	};
	//int main(int argc, _TCHAR* argv[])
	PARM getparm(int argc, _TCHAR* argv[]);	//������������ struct PARM �� ������ ���������� ������� main
};
