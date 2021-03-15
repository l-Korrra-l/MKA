#pragma once
#define ID_MAXSIZE		5				// максимальное количество сиволов в идентификаторе - пока увеличено временно
#define TI_MAXSIZE		4096			// максимальное количество строк в таблице идентификаторов
#define TI_INT_DEFAULT	0x00000000		// значение по умолчанию дл€ типа integer
#define TI_STR_DEFAULT	0x00			// значение по умолчанию дл€ типа string
#define TI_NULLIDX		0xffffffff		// нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE	255
#include  <cwchar>
namespace IT							// таблица идентификаторов
{
	enum IDDATATYPE { INT = 1, STR = 2 };		// типы данных идентификатов : integer, string
	enum IDTYPE { V = 1, F = 2, P = 3, L = 3 }; // типы идентификаторов : переменна€, функци€, параметр, литерал
	enum LITERALTYPE {I = 1,S = 2,O = 3,B=4};
	struct Entry								// строка таблицы идентификаторов
	{
		int idxfirstLE;							// индекс первой строки в таблице идентификатором
		char id[ID_MAXSIZE + 1];					// идентификатор (автоматически усекаетс€ до ID_MAXSIZE)
		IDDATATYPE iddatatype;					// тип данных
		IDTYPE idtype;							// тип идентикатора
		LITERALTYPE littype;
		Entry* view = NULL;
		union
		{
			int vint;							// значение integer
			struct
			{
				char len;						// количесво символов в string charlene;
				char str[TI_STR_MAXSIZE - 1];	// символы string
			} vstr[TI_STR_MAXSIZE];				// значение string
		} value;								// значение идентификатора
	};

	struct IdTable						// экземпл€р таблицы индентификаторов
	{
		int maxsize;					// емкость таблицы индентификаторов < TI_MAXSIZE
		int size;						// текущий размер таблицыиндентификаторов < maxsize
		Entry* table;					// массив строк таблицы индентификаторов
	};

	IdTable	Create				//создать таблицу индентификаторов
	(
		int size				//емкость таблицы индентификаторов < TI_MAXSIZE
	);
	void Add(					//добавить строку в таблицу индентификаторов
		IdTable& idtable,		//экземпл€р таблицы индентификаторов
		Entry entry				//строка таблицы индентификаторов
	);

	Entry GetEntry(				// получить строку таблицы индентификаторов
		IdTable& idtable,
		int n					//номер получаемой строки
	);

	int IsId(						//возврат: номер строки (если есть), TI_NULLIDX(если нет)
		IdTable& idtable,		//экземпл€р таблицы идентификаторов
		char id[ID_MAXSIZE]		//идентификатор
	);

	void Delete(IdTable& idtable);	// удалить таблицу индентификаторов (освободить пам€ть)
};
