#include "IT.h"
#include "Error.h"
#include <string.h>

namespace IT {
	IdTable Create(int size) {
		if (size > TI_MAXSIZE) throw ERROR_THROW(102);
		IdTable idtable{ size, 0, new Entry[size] };
		return idtable;
	}

	void Add(IdTable& idtable, Entry entry) {
		if (idtable.size + 1 > TI_MAXSIZE)
			throw ERROR_THROW(102);
		idtable.table[idtable.size++] = entry;
	}

	Entry GetEntry(IdTable& idtable, int n) {
		return idtable.table[n];
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE]) {
		for (int i = 0; i < idtable.size; i++) {
			if (!strcmp(idtable.table[i].id, id))
				return idtable.table[i].idxfirstLE;
		}
		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable) {
		delete[] idtable.table;
		idtable.size = 0;
	}
}