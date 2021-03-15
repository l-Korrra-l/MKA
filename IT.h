#pragma once
#define ID_MAXSIZE		5				// ������������ ���������� ������� � �������������� - ���� ��������� ��������
#define TI_MAXSIZE		4096			// ������������ ���������� ����� � ������� ���������������
#define TI_INT_DEFAULT	0x00000000		// �������� �� ��������� ��� ���� integer
#define TI_STR_DEFAULT	0x00			// �������� �� ��������� ��� ���� string
#define TI_NULLIDX		0xffffffff		// ��� �������� ������� ���������������
#define TI_STR_MAXSIZE	255
#include  <cwchar>
namespace IT							// ������� ���������������
{
	enum IDDATATYPE { INT = 1, STR = 2 };		// ���� ������ ������������� : integer, string
	enum IDTYPE { V = 1, F = 2, P = 3, L = 3 }; // ���� ��������������� : ����������, �������, ��������, �������
	enum LITERALTYPE {I = 1,S = 2,O = 3,B=4};
	struct Entry								// ������ ������� ���������������
	{
		int idxfirstLE;							// ������ ������ ������ � ������� ���������������
		char id[ID_MAXSIZE + 1];					// ������������� (������������� ��������� �� ID_MAXSIZE)
		IDDATATYPE iddatatype;					// ��� ������
		IDTYPE idtype;							// ��� ������������
		LITERALTYPE littype;
		Entry* view = NULL;
		union
		{
			int vint;							// �������� integer
			struct
			{
				char len;						// ��������� �������� � string charlene;
				char str[TI_STR_MAXSIZE - 1];	// ������� string
			} vstr[TI_STR_MAXSIZE];				// �������� string
		} value;								// �������� ��������������
	};

	struct IdTable						// ��������� ������� ����������������
	{
		int maxsize;					// ������� ������� ���������������� < TI_MAXSIZE
		int size;						// ������� ������ ����������������������� < maxsize
		Entry* table;					// ������ ����� ������� ����������������
	};

	IdTable	Create				//������� ������� ����������������
	(
		int size				//������� ������� ���������������� < TI_MAXSIZE
	);
	void Add(					//�������� ������ � ������� ����������������
		IdTable& idtable,		//��������� ������� ����������������
		Entry entry				//������ ������� ����������������
	);

	Entry GetEntry(				// �������� ������ ������� ����������������
		IdTable& idtable,
		int n					//����� ���������� ������
	);

	int IsId(						//�������: ����� ������ (���� ����), TI_NULLIDX(���� ���)
		IdTable& idtable,		//��������� ������� ���������������
		char id[ID_MAXSIZE]		//�������������
	);

	void Delete(IdTable& idtable);	// ������� ������� ���������������� (���������� ������)
};
