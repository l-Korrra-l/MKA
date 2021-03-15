#pragma once
#define IN_MAX_LEN_TEXT 1024*1024	// максимальный размер исходного кода = 1MB 
#define IN_CODE_ENDL '\n'			//символ конца строки 
#define DELIM '|' //убрано n11
#define IN_CODE_N '\0'			//символ конца 
#define SIZE 1000
#define QUOTE '\''
#define  delSpace(a,b) ((a==' '&&b==' ')||(b == ' ' && (a == ';' || a == ',' || a == '}' || a == '{' || a == '(' || a == ')' || a == '=' || a == '+' || a == '-' || a == '/' || a == '*')))?true:false
#define  delAfterSpace(a,b) ((a == ' ' && (b == ';' || b == ',' || b == '}' || b == '{' || b == '(' || b == ')' || b == '=' || b == '+' || b == '-' || b == '/' || b == '*')))?true:false
// таблица проверки входной информации, индекс = код (Windows-1251) символа
// значения IN::F - запрещенный символ, IN::T - разрешенный символ, IN::I - игнорировать (не вводить),
	//если 0 <= значение < 256 - то вводится данное значение
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
	struct IN	//исходные данные
	{
		enum { T = 1024, F = 2048, I = 4096, R=8192, S=3072}; // Т - допустимый символ, F - недопустимый, I - игнорировать, иначе заменить, R-русские символы, S-Space
		int size;		// размер исходного кода
		int lines;		// количество строк
		int ignor;		// количество проигнорированных символов
		unsigned char* text;// исходный код (Windows - 1251)
		int code[256] = IN_CODE_TABLE;// таблица проверки: T, F, I новое значение
		long long int counterLexem;
		void change();
		void EditText();
	};
	IN getin(wchar_t infile[]);	// ввести и проверить входной поток	
};