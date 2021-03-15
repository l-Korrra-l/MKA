#include "polska.h"
#include "LAN.h"
#include "LT.h"
#include "IT.h"
#include "Error.h"
#include <string>
#include <vector>
#include <stack>

bool PN::searchExpression(Lan::LAN Tables) {
	bool fl = false;
	for (int i = 0; i < Tables.lexTable.size; i++) {
		if (Tables.lexTable.table[i].lexema == LEX_ASSIGNMENT) {
			fl = PolishNotation(++i, Tables);
		}if (Tables.lexTable.table[i].lexema == LEX_RETURN) {
			fl = PolishNotation(++i, Tables);
		}		
	}
	return fl;
}
bool PN::PolishNotation(int lextable_pos, Lan::LAN& tab) {
	std::stack<LT::Entry> st;							
	LT::Entry outstr[20];								
	int len = 0,									
		lenout = 0,									
		semicolonid=0;								
	char skob = 0;
	int counterBracelet=0;
	int indoffunk;						
	for (int i = lextable_pos; tab.lexTable.table[i].lexema != LEX_SEMICOLON; i++) {
		len = i;									
		semicolonid = i + 1;
	}
	len++;
	for (int i = lextable_pos; i < len; i++) {
		switch (tab.lexTable.table[i].lexema)
		{
		case LEX_CLOSEBRACKET:					
		{
			while (st.top().lexema != '(') {
				outstr[lenout++] = st.top();		
				skob++;
				st.pop();	
				counterBracelet++;
			}
			st.pop();							
			break;
		}
		case LEX_ID:								
		case LEX_LITERAL:							
		{
			if (tab.lexTable.table[i + 1].lexema == LEX_OPENBRACKET && tab.lexTable.table[i + 3].lexema != LEX_COMMA) {
				indoffunk = i;									
				i += 2;												
				while (tab.lexTable.table[i].lexema != LEX_CLOSEBRACKET) {
					if (tab.lexTable.table[i].lexema != LEX_COMMA) {
						outstr[lenout++] = tab.lexTable.table[i++];
					}
					else
					{
						skob++;
						i++;					
					}
				}
				outstr[lenout++] = tab.lexTable.table[indoffunk];
				skob += 2;

			}
			else {
				outstr[lenout++] = tab.lexTable.table[i];
			}
			break;
		}
		case LEX_OPENBRACKET:
		{
			if (tab.lexTable.table[i + 2].lexema == LEX_COMMA) return 1;
			st.push(tab.lexTable.table[i]);
			skob++;
			counterBracelet++;
			break;
		}

		case LEX_PLUS:
		case LEX_MINUS:
		case LEX_STAR:
		case LEX_SLASH:
		case LEX_RETURN:
		{
			if (!st.size())					
				st.push(tab.lexTable.table[i]);
			else {
				int pr, id;
				id = st.top().lexema;
				pr = prior(id);				
				if (prior(tab.lexTable.table[i].lexema) > pr)
					st.push(tab.lexTable.table[i]);
				else {									
													
					while (st.size() && prior(tab.lexTable.table[i].lexema) <= prior(id))
					{
						outstr[lenout] = st.top();
						st.pop();
						lenout++;
					}
					st.push(tab.lexTable.table[i]);
				}
			}
			break;
		}
		}
	}
	while (st.size()) {
		outstr[lenout++] = st.top();								
		st.pop();
	}
	bool eol = false;
	int pos=0;
	for (int i = lextable_pos, k = 0; i < lextable_pos + lenout; i++) {
		if ((tab.lexTable.table[i].lexema == LEX_ID && tab.idTable.table[tab.lexTable.table[i].idxTI].idtype == 2) ) {}
		else {			
				tab.lexTable.table[i] = outstr[k];
				k++;			
		}
	}
	tab.lexTable.table[lextable_pos + lenout] = tab.lexTable.table[semicolonid];
	if (lenout>1)
	{
		for (int i = lextable_pos+lenout+1; i <= len; i++)
		{
			tab.lexTable.table[i].lexema = LEX_FREE;
		}
	}
	return 1;
}
int PN::prior(char l) {
	if (l == ')' || l == '(')
		return 1;
	if (l == '+' || l == '-')
		return 2;
	if (l == '*' || l == '/')
		return 3;
	if (l == ':')
		return 4;
}

void PN::Print(Lan::LAN tables)
{	
	std::ofstream pol("PolishNotation.txt");
	for (int i = 0; i < tables.lexTable.size; i++) {
		pol << tables.lexTable.table[i].lexema;
		if (tables.lexTable.table[i].sn != tables.lexTable.table[i + 1].sn) pol << std::endl;
	}
}


