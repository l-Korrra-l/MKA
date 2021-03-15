#include "LT.h"
#include "Error.h"

namespace LT {
	LexTable Create(int size) {
		if (size > LT_MAXSIZE) throw ERROR_THROW(103);
		LexTable lextable{ size, 0, new Entry[size] };
		return lextable;
	}

	void Add(LexTable& lextable, Entry entry) {
		if (lextable.size + 1 > LT_MAXSIZE)
			throw ERROR_THROW(103);
		lextable.table[lextable.size++] = entry;
	}

	Entry GetEntry(LexTable& lextable, int n) {
		return lextable.table[n];
	}

	void Delete(LexTable& lextable) {
		delete[] lextable.table;
		lextable.size = 0;
	}
}