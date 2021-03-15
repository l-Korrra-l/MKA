#pragma once
#define LEXEMA_FIXSIZE		1
#define LT_MAXSIZE			4096
#define LT_TI_NULLIDX		0xFFFFFFFF
#define LEX_INTEGER			't'
#define LEX_STRING			't'
#define LEX_ID				'i'
#define LEX_IF				'?'
#define LEX_ELIF			'e'
#define LEX_ELSE			's'
#define LEX_LITERAL			'l'
#define LEX_FUNCTION		'f'
#define LEX_DECLARE			'd'
#define LEX_RETURN			'r'
#define LEX_PRINT			'p'
#define LEX_MAIN			'm'
#define LEX_SEMICOLON		';'
#define LEX_COMMA			','
#define LEX_LEFTBRACE		'{'
#define LEX_RIGHTBRACE		'}'
#define LEX_OPENBRACKET		'('
#define LEX_CLOSEBRACKET	')'
#define LEX_PLUS			'+' // +
#define LEX_MINUS			'-' // -
#define	LEX_STAR			'*' // *
#define LEX_SLASH			'/' // /
#define LEX_EQUAL			'o' // ==
#define LEX_NEQUAL			'z' // !=
#define	LEX_LEQUAL			'h' // <=
#define LEX_MEQUAL			'k' // >=
#define	LEX_LESS			'<' // <
#define LEX_MORE			'>' // >
#define LEX_ASSIGNMENT		'='
#define LEX_STARTBLOCK		'['
#define LEX_ENDBLOCK		']'
#define LEX_FREE			'#'	//��� ���������


namespace LT
{                     
		struct Entry                     // ������ ������� ������
		{
			char lexema;				 // ������
			int sn;                      // ����� ������ � �������� ������
			int idxTI;                   // ������ � ������� ��������������� ��� LT_TI_NULLIDX
			char vType = '`';            //���
			int priority = -1;           //���������
		};

		struct LexTable                  // ��������� ������� ������ 
		{
			int maxsize;                 // ������� ������� ������ < LT_MAXSIZE
			int size;                    // ������� ������ ������� ������ < maxsize
			Entry* table;                // ������ ����� ������� ������
			unsigned char* LexemText;
		};

		LexTable Create(                 // ������� ������� ������
			int size                     // ������� ������� ������ < LT_MAXSIZE
		);

		void Add(                        // �������� ������ � ������� ������
			LexTable& lextable,          // ��������� ������� ������
			Entry entry                  // ������ ������� ������
		);

		Entry GetEntry(                  // �������� ������ ������� ������
			LexTable& lextable,          // ��������� ������� ������
			int n                        // ����� ���������� ������
		);

	void Delete(LexTable &lextable);
}