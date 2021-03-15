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
#define LEX_FREE			'#'	//дл€ удаленных


namespace LT
{                     
		struct Entry                     // строка таблицы лексем
		{
			char lexema;				 // лесема
			int sn;                      // номер строки в исходном тексте
			int idxTI;                   // индекс в таблице идентификаторов или LT_TI_NULLIDX
			char vType = '`';            //тип
			int priority = -1;           //приоритет
		};

		struct LexTable                  // экземпл€р таблицы лексем 
		{
			int maxsize;                 // емкость таблицы лексем < LT_MAXSIZE
			int size;                    // текущий размер таблицы лексем < maxsize
			Entry* table;                // массив строк таблицы лексем
			unsigned char* LexemText;
		};

		LexTable Create(                 // создать таблицу лексем
			int size                     // емкость таблицы лексем < LT_MAXSIZE
		);

		void Add(                        // добавить строку в таблицу лексем
			LexTable& lextable,          // экземпл€р таблицы лексем
			Entry entry                  // строка таблицы лексем
		);

		Entry GetEntry(                  // получить строку таблицы лексем
			LexTable& lextable,          // экземпл€р таблицы лексем
			int n                        // номер получаемой строки
		);

	void Delete(LexTable &lextable);
}