#pragma once
#define IN_MAX_LEN_TEXT 1024*1024	// ������������ ������ ��������� ���� = 1MB 
#define IN_CODE_ENDL '\n'			//������ ����� ������ 
#define DELIM '|' //������ n11
#define IN_CODE_N '\0'			//������ ����� 
#define SIZE 1000
#define QUOTE '\''
#define  delSpace(a,b) ((a==' '&&b==' ')||(b == ' ' && (a == ';' || a == ',' || a == '}' || a == '{' || a == '(' || a == ')' || a == '=' || a == '+' || a == '-' || a == '/' || a == '*')))?true:false
#define  delAfterSpace(a,b) ((a == ' ' && (b == ';' || b == ',' || b == '}' || b == '{' || b == '(' || b == ')' || b == '=' || b == '+' || b == '-' || b == '/' || b == '*')))?true:false
// ������� �������� ������� ����������, ������ = ��� (Windows-1251) �������
// �������� IN::F - ����������� ������, IN::T - ����������� ������, IN::I - ������������ (�� �������),
	//���� 0 <= �������� < 256 - �� �������� ������ ��������
#define IN_CODE_TABLE {\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F,/*0-15*/\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,/*16-31*/\
	IN::S, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::T,/*32-47*/\
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F,/*48-63*/\
	IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,/*64-79*/\
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::T,/*80-95*/\
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,/*96-111*/\
	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::F,/*112-127*/\
	/*0		  1		2		3	   4	  5		 6		7	   8	  9		10	   11	  12	 13		14	   15*/\
	IN::F, IN::F, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,/*128-143*/\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,/*144-159*/\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,/*160-175*/\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,/*176-191*/\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,/*192-207*/\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,/*208-223*/\
	IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R,/*224-239*/\
	IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R, IN::R,/*240-255*/\
			}

namespace In
{
	struct IN	//�������� ������
	{
		enum { T = 1024, F = 2048, I = 4096, R=8192, S=3072}; // � - ���������� ������, F - ������������, I - ������������, ����� ��������, R-������� �������, S-Space
		int size;		// ������ ��������� ����
		int lines;		// ���������� �����
		int ignor;		// ���������� ����������������� ��������
		unsigned char* text;// �������� ��� (Windows - 1251)
		int code[256] = IN_CODE_TABLE;// ������� ��������: T, F, I ����� ��������
		long long int counterLexem;
		void change();
		void EditText();
	};
	IN getin(wchar_t infile[]);	// ������ � ��������� ������� �����	
};