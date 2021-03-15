#pragma once
#include "FST.h"
#include "LAN.h"

struct PN {
	bool PolishNotation(int lextable_pos, Lan::LAN& table);
	bool searchExpression(Lan::LAN tables);
	int prior(char);
	void Print(Lan::LAN tables);
};