#include "LAN.h"
#include "Error.h"
#include "FST.h"
#include "Expr.h"

#include <iostream>
#include <iomanip>
#include <locale>
#include <stack>
#include <string>
#include <list>
namespace Lan {
	std::stack <IT::Entry> stackCall;
	int counterLiteral = 0;
	int counterTokenInProgram = 0;
	int counterID = 0;
	bool ifBrace=false;
	bool ifFlag=false;
	bool ifBrack = false;
	char ViewName[10];
	int CountEq=0;
	char* stringToChar(std::string str) {
		char* massChar = new char[255];
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == '\\' && str[i + 1] == '"') continue;
			massChar[i] = str[i];
		}
		massChar[str.length()] = '\0';
		return massChar;
	}
	void LAN::InspectSymbol(const unsigned char symbol) { //функция проходит по символам
		if (WordStates == State::StringLiteral) {
			if (symbol == QUOTE) {
				Confirm(); //закончить слово
				WordStates = State::Token;
			}
			return;
		}
		else if (symbol == ' ') {
			Confirm();
		}
		else if (CurrentWord.empty()) {
			if (symbol == QUOTE)	WordStates = State::StringLiteral;
			else CurrentWord += symbol;
		}
		else
		{
			unsigned char PreviousSymbol = CurrentWord[CurrentWord.size() - 1]; //проверяет в зависимости от предыдущего символа
		 if (isdigit(PreviousSymbol)|| PreviousSymbol=='x'|| PreviousSymbol == 'b') { //если цифра предыдущий
				if (isdigit(symbol) || symbol == 'x'|| symbol == 'b') {			//если цифра текущий
					CurrentWord += symbol;
				}
				else if (!isalpha(symbol)) {	//если + - / *
					Confirm();
					CurrentWord += symbol;
				}
				else throw ERROR_THROW_IN(2, currentLine, currentPosition);
		 }
		 else
			 if (isalpha(PreviousSymbol)) //если a-z предыдущий
			 {
				 if (isalpha(symbol)) {	 //если a-z текущий
					 CurrentWord += symbol;
				 }
				 else if (!isdigit(symbol)) { //	если + - / *
						 Confirm();
						 CurrentWord += symbol;
				 }
				 else throw ERROR_THROW_IN(2, currentLine, currentPosition);
			 }
			 else { //если + - / *
				  if (ifFlag&&symbol=='=')
				 { 
					 CurrentWord += symbol;
                     Confirm();
					 CountEq = 0;
					 ifFlag = false;
				 }else{ 
					 Confirm();
					 CurrentWord += symbol; 
				 }

			 }
		}
		currentPosition++;
		if (symbol == IN_CODE_ENDL) {
			CurrentWord.clear();
			currentPosition = 1;
			currentLine++;
		}
	}

	void LAN::SetType()
	{
		if (CurrentWord == "string")	TypeIsStringOrInteger = Type::String;
		else if (CurrentWord == "integer") TypeIsStringOrInteger = Type::Integer;
	}
	LT::Entry createStructLexem(char type, int number, int numberInTableId, char vType = '$', int priority = -1) {
		LT::Entry newLexem;
		newLexem.lexema = type;
		newLexem.sn = number;
		newLexem.idxTI = numberInTableId;
		newLexem.vType = vType;
		newLexem.priority = priority;
		return newLexem;
	}
	IT::Entry createStructId(char* id, int line, int* typeData, int* typeID, int* littype, char lexema[TI_STR_MAXSIZE] = NULL, IT::Entry* view = NULL) {
		IT::Entry newItEntry;
		int counterSymbol = 0;
		//int* typeD = (int*)typeData;
		for (int i = 0; i < strlen(id); i++)
		{
			newItEntry.id[i] = id[i];
			counterSymbol++;
		}
		newItEntry.id[counterSymbol] = '\0';
		if (typeData) newItEntry.iddatatype = (IT::IDDATATYPE)*typeData;
		if (typeID) newItEntry.idtype = (IT::IDTYPE)*typeID;
		if (littype) newItEntry.littype = (IT::LITERALTYPE)*littype;
		newItEntry.idxfirstLE = line;
		newItEntry.view = view;
		if ((int)newItEntry.idtype != 4) {
			if ((int)newItEntry.iddatatype == 1)
			{
				newItEntry.value.vint = TI_INT_DEFAULT;
			}
			else
			{
				newItEntry.value.vstr->len = 0;
				newItEntry.value.vstr->str[0] = TI_STR_DEFAULT;
				newItEntry.value.vstr->str[1] = '\0';
			}
		}
		else
		{
			if (((int)newItEntry.iddatatype == 1) && ((int)newItEntry.littype == 1))
			{
				newItEntry.value.vint = atoi(lexema);
			}
			else if ((int)newItEntry.littype == 3) {
				std::string sVal = lexema;
				std::string result = sVal.substr(1, strlen(lexema) - 1);

				newItEntry.value.vint = atoi(stringToChar(result));
			}
			else
			{
				newItEntry.value.vstr->len = strlen(lexema);
				strcpy_s(newItEntry.value.vstr->str, lexema);
			}
		}
		return newItEntry;
	}
	void LAN::Inspect() { //занести в таблицу лексем, вызывается из Confirm
		//int* typeData, * typeID;
		int typeData=0;
		int typeID = 0;
		int numberOfstring=currentLine; // номер строки в исходном тексте
		int i = 0;
		char* text = stringToChar(CurrentWord);
		bool haveMain = false;
		bool haveIF = false;
		FST::FST fstInt(text, 8,
			FST::NODE(1, FST::RELATION('i', 1)),
			FST::NODE(1, FST::RELATION('n', 2)),
			FST::NODE(1, FST::RELATION('t', 3)),
			FST::NODE(1, FST::RELATION('e', 4)),
			FST::NODE(1, FST::RELATION('g', 5)),
			FST::NODE(1, FST::RELATION('e', 6)),
			FST::NODE(1, FST::RELATION('r', 7)),
			FST::NODE());
		FST::FST fstFunc(text, 9,
			FST::NODE(1, FST::RELATION('f', 1)),
			FST::NODE(1, FST::RELATION('u', 2)),
			FST::NODE(1, FST::RELATION('n', 3)),
			FST::NODE(1, FST::RELATION('c', 4)),
			FST::NODE(1, FST::RELATION('t', 5)),
			FST::NODE(1, FST::RELATION('i', 6)),
			FST::NODE(1, FST::RELATION('o', 7)),
			FST::NODE(1, FST::RELATION('n', 8)),
			FST::NODE());
		FST::FST fstStr(text, 7,
			FST::NODE(1, FST::RELATION('s', 1)),
			FST::NODE(1, FST::RELATION('t', 2)),
			FST::NODE(1, FST::RELATION('r', 3)),
			FST::NODE(1, FST::RELATION('i', 4)),
			FST::NODE(1, FST::RELATION('n', 5)),
			FST::NODE(1, FST::RELATION('g', 6)),
			FST::NODE());
		FST::FST fstDec(text, 8,
			FST::NODE(1, FST::RELATION('d', 1)),
			FST::NODE(1, FST::RELATION('e', 2)),
			FST::NODE(1, FST::RELATION('c', 3)),
			FST::NODE(1, FST::RELATION('l', 4)),
			FST::NODE(1, FST::RELATION('a', 5)),
			FST::NODE(1, FST::RELATION('r', 6)),
			FST::NODE(1, FST::RELATION('e', 7)),
			FST::NODE());
		FST::FST fstReturn(text, 7,
			FST::NODE(1, FST::RELATION('r', 1)),
			FST::NODE(1, FST::RELATION('e', 2)),
			FST::NODE(1, FST::RELATION('t', 3)),
			FST::NODE(1, FST::RELATION('u', 4)),
			FST::NODE(1, FST::RELATION('r', 5)),
			FST::NODE(1, FST::RELATION('n', 6)),
			FST::NODE());
		FST::FST fstPrint(text, 6,
			FST::NODE(1, FST::RELATION('p', 1)),
			FST::NODE(1, FST::RELATION('r', 2)),
			FST::NODE(1, FST::RELATION('i', 3)),
			FST::NODE(1, FST::RELATION('n', 4)),
			FST::NODE(1, FST::RELATION('t', 5)),
			FST::NODE());
		FST::FST fstMain(text, 5,
			FST::NODE(1, FST::RELATION('m', 1)),
			FST::NODE(1, FST::RELATION('a', 2)),
			FST::NODE(1, FST::RELATION('i', 3)),
			FST::NODE(1, FST::RELATION('n', 4)),
			FST::NODE());

		FST::FST fstSemicolon(text, 2,
			FST::NODE(1, FST::RELATION(';', 1)),
			FST::NODE()
		);
		FST::FST fstComma(text, 2,
			FST::NODE(1, FST::RELATION(',', 1)),
			FST::NODE()
		);
		FST::FST fstLeftbrace(text, 2,
			FST::NODE(1, FST::RELATION('{', 1)),
			FST::NODE()
		);
		FST::FST fstBracelet(text, 2,
			FST::NODE(1, FST::RELATION('}', 1)),
			FST::NODE()
		);
		FST::FST fstLefthesis(text, 2,
			FST::NODE(1, FST::RELATION('(', 1)),
			FST::NODE()
		);
		FST::FST fstRighthesis(text, 2,
			FST::NODE(1, FST::RELATION(')', 1)),
			FST::NODE()
		);
		FST::FST fstPlus(text, 2,
			FST::NODE(1, FST::RELATION('+', 1)),
			FST::NODE()
		);
		FST::FST fstMinus(text, 2,
			FST::NODE(1, FST::RELATION('-', 1)),
			FST::NODE()
		);
		FST::FST fstStar(text, 2,
			FST::NODE(1, FST::RELATION('*', 1)),
			FST::NODE()
		);
		FST::FST fstDirslash(text, 2,
			FST::NODE(1, FST::RELATION('/', 1)),
			FST::NODE()
		);

		FST::FST fstStartBlock(text, 2,
			FST::NODE(1, FST::RELATION('[', 1)),
			FST::NODE()
		);

		FST::FST fstEndBlock(text, 2,
			FST::NODE(1, FST::RELATION(']', 1)),
			FST::NODE()
		);

		FST::FST fstIf(text, 3,
			FST::NODE(1, FST::RELATION('i', 1)),
			FST::NODE(1, FST::RELATION('f', 2)),
			FST::NODE()
		);

		FST::FST fstElif(text, 5,
			FST::NODE(1, FST::RELATION('e', 1)),
			FST::NODE(1, FST::RELATION('l', 2)),
			FST::NODE(1, FST::RELATION('i', 3)),
			FST::NODE(1, FST::RELATION('f', 4)),
			FST::NODE()
		);

		FST::FST fstElse(text, 5,
			FST::NODE(1, FST::RELATION('e', 1)),
			FST::NODE(1, FST::RELATION('l', 2)),
			FST::NODE(1, FST::RELATION('s', 3)),
			FST::NODE(1, FST::RELATION('e', 4)),
			FST::NODE()
		);

		FST::FST fstOctLit(text, 4,
			FST::NODE(1, FST::RELATION('0', 1)),
			FST::NODE(1, FST::RELATION('x', 2)),
			FST::NODE(15, FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2), FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2),
				FST::RELATION('0', 3), FST::RELATION('1', 3), FST::RELATION('2', 3), FST::RELATION('3', 3), FST::RELATION('4', 3), FST::RELATION('5', 3), FST::RELATION('6', 3), FST::RELATION('7', 3)),
			FST::NODE()
		);
		FST::FST fstBinLit(text, 4,
			FST::NODE(1, FST::RELATION('0', 1)),
			FST::NODE(1, FST::RELATION('b', 2)),
			FST::NODE(4, FST::RELATION('0', 2), FST::RELATION('1', 2),
				FST::RELATION('0', 3), FST::RELATION('1', 3)),
			FST::NODE()
		);

		FST::FST fstLiteralOfInteger(text, 2,
			FST::NODE(19, FST::RELATION('1', 0), FST::RELATION('2', 0), FST::RELATION('3', 0), FST::RELATION('4', 0), FST::RELATION('5', 0), FST::RELATION('6', 0),
				FST::RELATION('7', 0), FST::RELATION('8', 0), FST::RELATION('9', 0),
				FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1), FST::RELATION('6', 1),
				FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1)),
			FST::NODE()
		);
		FST::FST fstId(text, 2,
			FST::NODE(88,
				FST::RELATION('A', 0), FST::RELATION('B', 0), FST::RELATION('C', 0), FST::RELATION('D', 0), FST::RELATION('E', 0), FST::RELATION('F', 0),
				FST::RELATION('G', 0), FST::RELATION('H', 0), FST::RELATION('I', 0), FST::RELATION('J', 0), FST::RELATION('K', 0), FST::RELATION('L', 0),
				FST::RELATION('M', 0), FST::RELATION('N', 0), FST::RELATION('O', 0), FST::RELATION('P', 0), FST::RELATION('Q', 0), FST::RELATION('R', 0),
				FST::RELATION('S', 0), FST::RELATION('T', 0), FST::RELATION('U', 0), FST::RELATION('V', 0), FST::RELATION('W', 0), FST::RELATION('X', 0),
				FST::RELATION('Y', 0), FST::RELATION('Z', 0),
				FST::RELATION('a', 0), FST::RELATION('b', 0), FST::RELATION('c', 0), FST::RELATION('d', 0), FST::RELATION('e', 0), FST::RELATION('f', 0),
				FST::RELATION('g', 0), FST::RELATION('h', 0), FST::RELATION('i', 0), FST::RELATION('j', 0), FST::RELATION('k', 0), FST::RELATION('l', 0),
				FST::RELATION('m', 0), FST::RELATION('n', 0), FST::RELATION('o', 0), FST::RELATION('p', 0), FST::RELATION('q', 0), FST::RELATION('r', 0),
				FST::RELATION('s', 0), FST::RELATION('t', 0), FST::RELATION('u', 0), FST::RELATION('v', 0), FST::RELATION('w', 0), FST::RELATION('x', 0),
				FST::RELATION('y', 0), FST::RELATION('z', 0),
				FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1), FST::RELATION('f', 1),
				FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1), FST::RELATION('k', 1), FST::RELATION('l', 1),
				FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1), FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1),
				FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1),
				FST::RELATION('y', 1), FST::RELATION('z', 1),
				FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1),
				FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1)),
			FST::NODE()
		);
		FST::FST fstEqual(text, 2,
			FST::NODE(1, FST::RELATION('=', 1)),
			FST::NODE()
		);

		FST::FST fstMore(text, 2,
			FST::NODE(1, FST::RELATION('>', 1)),
			FST::NODE()
		);
		FST::FST fstLess(text, 2,
			FST::NODE(1, FST::RELATION('<', 1)),
			FST::NODE()
		);
		FST::FST fstMoreEq(text, 3,
			FST::NODE(1, FST::RELATION('>', 1)),
			FST::NODE(1, FST::RELATION('=', 2)),
			FST::NODE()
		);
		FST::FST fstlessEq(text, 3,
			FST::NODE(1, FST::RELATION('<', 1)),
			FST::NODE(1, FST::RELATION('=', 2)),
			FST::NODE()
		);
		FST::FST fstIsEq(text, 3,
			FST::NODE(1, FST::RELATION('=', 1)),
			FST::NODE(1, FST::RELATION('=', 2)),
			FST::NODE()
		);
		FST::FST fstNotEq(text, 3,
			FST::NODE(1, FST::RELATION('!', 1)),
			FST::NODE(1, FST::RELATION('=', 2)),
			FST::NODE()
		);
		FST::FST fstLiteralOfString(text, 3,
			FST::NODE(2, FST::RELATION('\'', 1), FST::RELATION('\'', 2)),
			FST::NODE(158,
				FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1), FST::RELATION('f', 1),
				FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1), FST::RELATION('k', 1), FST::RELATION('l', 1),
				FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1), FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1),
				FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1),
				FST::RELATION('y', 1), FST::RELATION('z', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
				FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1), FST::RELATION('0', 1),

				FST::RELATION('а', 1), FST::RELATION('б', 1), FST::RELATION('в', 1), FST::RELATION('г', 1), FST::RELATION('д', 1), FST::RELATION('е', 1),
				FST::RELATION('ё', 1), FST::RELATION('ж', 1), FST::RELATION('з', 1), FST::RELATION('и', 1), FST::RELATION('й', 1), FST::RELATION('к', 1),
				FST::RELATION('л', 1), FST::RELATION('м', 1), FST::RELATION('н', 1), FST::RELATION('о', 1), FST::RELATION('п', 1), FST::RELATION('р', 1),
				FST::RELATION('с', 1), FST::RELATION('т', 1), FST::RELATION('у', 1), FST::RELATION('ф', 1), FST::RELATION('х', 1), FST::RELATION('ц', 1),
				FST::RELATION('ч', 1), FST::RELATION('ш', 1), FST::RELATION('щ', 1), FST::RELATION('ъ', 1), FST::RELATION('ы', 1), FST::RELATION('ь', 1),
				FST::RELATION('э', 1), FST::RELATION('ю', 1), FST::RELATION('я', 1), FST::RELATION(' ', 1), FST::RELATION('.', 1), FST::RELATION(',', 1),
				FST::RELATION('?', 1), FST::RELATION('!', 1), FST::RELATION(';', 1), FST::RELATION(':', 1), FST::RELATION('-', 1), FST::RELATION(')', 1),
				FST::RELATION('(', 1),

				FST::RELATION('a', 2), FST::RELATION('b', 2), FST::RELATION('c', 2), FST::RELATION('d', 2), FST::RELATION('e', 2), FST::RELATION('f', 2),
				FST::RELATION('g', 2), FST::RELATION('h', 2), FST::RELATION('i', 2), FST::RELATION('j', 2), FST::RELATION('k', 2), FST::RELATION('l', 2),
				FST::RELATION('m', 2), FST::RELATION('n', 2), FST::RELATION('o', 2), FST::RELATION('p', 2), FST::RELATION('q', 2), FST::RELATION('r', 2),
				FST::RELATION('s', 2), FST::RELATION('t', 2), FST::RELATION('u', 2), FST::RELATION('v', 2), FST::RELATION('w', 2), FST::RELATION('x', 2),
				FST::RELATION('y', 2), FST::RELATION('z', 2), FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2),
				FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2), FST::RELATION('9', 2), FST::RELATION('0', 2),

				FST::RELATION('а', 2), FST::RELATION('б', 2), FST::RELATION('в', 2), FST::RELATION('г', 2), FST::RELATION('д', 2), FST::RELATION('е', 2),
				FST::RELATION('ё', 2), FST::RELATION('ж', 2), FST::RELATION('з', 2), FST::RELATION('и', 2), FST::RELATION('й', 2), FST::RELATION('к', 2),
				FST::RELATION('л', 2), FST::RELATION('м', 2), FST::RELATION('н', 2), FST::RELATION('о', 2), FST::RELATION('п', 2), FST::RELATION('р', 2),
				FST::RELATION('с', 2), FST::RELATION('т', 2), FST::RELATION('у', 2), FST::RELATION('ф', 2), FST::RELATION('х', 2), FST::RELATION('ц', 2),
				FST::RELATION('ч', 2), FST::RELATION('ш', 2), FST::RELATION('щ', 2), FST::RELATION('ъ', 2), FST::RELATION('ы', 2), FST::RELATION('ь', 2),
				FST::RELATION('э', 2), FST::RELATION('ю', 2), FST::RELATION('я', 2), FST::RELATION(' ', 2), FST::RELATION('.', 2), FST::RELATION(',', 2),
				FST::RELATION('?', 2), FST::RELATION('!', 2), FST::RELATION(';', 2), FST::RELATION(':', 2), FST::RELATION('-', 2), FST::RELATION(')', 2),
				FST::RELATION('(', 2)),
			FST::NODE(1, FST::RELATION('\'', 3)),
			FST::NODE());

		if (execute(fstInt)) {
			Add(lexTable, createStructLexem(LEX_INTEGER, numberOfstring, LT_TI_NULLIDX));
			typeData = 1;
			return;
		}
		if (execute(fstStr)) {
			Add(lexTable, createStructLexem(LEX_STRING, numberOfstring, LT_TI_NULLIDX));
			typeData = 2;
			return;
		}
		if (execute(fstDec)) {
			Add(lexTable, createStructLexem(LEX_DECLARE, numberOfstring, LT_TI_NULLIDX));
			typeID = 2;
			return;
		}
		if (execute(fstFunc)) {
			Add(lexTable, createStructLexem(LEX_FUNCTION, numberOfstring, LT_TI_NULLIDX));
			return;
		}
		if (execute(fstReturn)) {
			Add(lexTable, createStructLexem(LEX_RETURN, numberOfstring, LT_TI_NULLIDX));
			return;
		}
		if (execute(fstPrint)) {
			Add(lexTable, createStructLexem(LEX_PRINT, numberOfstring, LT_TI_NULLIDX));
			return;
		}
		if (execute(fstMain)) {
			Add(lexTable, createStructLexem(LEX_MAIN, numberOfstring, LT_TI_NULLIDX));
			haveMain = true;
			char  gl[6] = "Main";
			stackCall.push(createStructId(gl, -1, NULL, NULL, NULL, gl, NULL));
			return;
		}
		if (execute(fstSemicolon)) {
			Add(lexTable, createStructLexem(LEX_SEMICOLON, numberOfstring, LT_TI_NULLIDX));
			return;
		}
		if (execute(fstComma)) {
			Add(lexTable, createStructLexem(LEX_COMMA, numberOfstring, LT_TI_NULLIDX));
			return;
		}
		if (execute(fstLeftbrace)) {
			Add(lexTable, createStructLexem(LEX_LEFTBRACE, numberOfstring, LT_TI_NULLIDX));
			return;
		}
		if (execute(fstBracelet)) {
			Add(lexTable, createStructLexem(LEX_RIGHTBRACE, numberOfstring, LT_TI_NULLIDX));
			//stackCall.pop();
			return;
		}
		if (execute(fstLefthesis)) {
			Add(lexTable, createStructLexem(LEX_OPENBRACKET, numberOfstring, LT_TI_NULLIDX, '(', 1));
			if (typeID == 2) typeID = 3;
			return;
		}
		if (execute(fstRighthesis)) {
			Add(lexTable, createStructLexem(LEX_CLOSEBRACKET, numberOfstring, LT_TI_NULLIDX, ')', 1));
			if (typeID == 3) typeID = -1;
			return;
		}
		if (execute(fstPlus)) {
			Add(lexTable, createStructLexem(LEX_PLUS, numberOfstring, LT_TI_NULLIDX, '+', 2));
			return;
		}
		if (execute(fstMinus)) {
			Add(lexTable, createStructLexem(LEX_MINUS, numberOfstring, LT_TI_NULLIDX, '-', 2));
			return;
		}
		if (execute(fstStar)) {
			Add(lexTable, createStructLexem(LEX_STAR, numberOfstring, LT_TI_NULLIDX, '*', 3));
			return;
		}
		if (execute(fstDirslash)) {
			Add(lexTable, createStructLexem(LEX_SLASH, numberOfstring, LT_TI_NULLIDX, '*', 3));
			return;
		}
		if (execute(fstEqual)) {
			Add(lexTable, createStructLexem(LEX_ASSIGNMENT, numberOfstring, LT_TI_NULLIDX, '='));
			return;
		}
		if (execute(fstMore)) {
			Add(lexTable, createStructLexem(LEX_MORE, numberOfstring, LT_TI_NULLIDX, '>', 2));
			return;
		}
		if (execute(fstMoreEq)) {
			Add(lexTable, createStructLexem(LEX_MEQUAL, numberOfstring, LT_TI_NULLIDX, 'k', 2));
			return;
		}
		if (execute(fstLess)) {
			Add(lexTable, createStructLexem(LEX_LESS, numberOfstring, LT_TI_NULLIDX, '<', 2));
			return;
		}
		if (execute(fstlessEq)) {
			Add(lexTable, createStructLexem(LEX_LEQUAL, numberOfstring, LT_TI_NULLIDX, 'h', 2));
			return;
		}
		if (execute(fstIsEq)) {
			Add(lexTable, createStructLexem(LEX_EQUAL, numberOfstring, LT_TI_NULLIDX, 'o', 2));
			return;
		}
		if (execute(fstNotEq)) {
			Add(lexTable, createStructLexem(LEX_NEQUAL, numberOfstring, LT_TI_NULLIDX, 'z', 2));
			return;
		}
		if (execute(fstStartBlock)) {
			Add(lexTable, createStructLexem(LEX_STARTBLOCK, numberOfstring, LT_TI_NULLIDX));
			return;
		}
		if (execute(fstEndBlock)) {
			Add(lexTable, createStructLexem(LEX_ENDBLOCK, numberOfstring, LT_TI_NULLIDX));
			return;
		}
		if (execute(fstIf)) {
			Add(lexTable, createStructLexem(LEX_IF, numberOfstring, LT_TI_NULLIDX));
			ifFlag = true;
			haveIF = true;
			return;
		}
		if (execute(fstElif)) {
			Add(lexTable, createStructLexem(LEX_ELIF, numberOfstring, LT_TI_NULLIDX));
			ifFlag = true;
			haveIF = false;
			return;
		}
		if (execute(fstElse)) {
			Add(lexTable, createStructLexem(LEX_ELSE, numberOfstring, LT_TI_NULLIDX));
			return;
		}
		if (execute(fstOctLit))
		{
			char buff[10];
			int a = 0;
			strcpy_s(buff, &(CurrentWord.c_str())[2]);
			a = strtoll(buff, NULL, 8);
			CurrentWord = std::to_string(a);
		}
		if (execute(fstBinLit))
		{
			char buff[10];
			int a = 0;
			strcpy_s(buff, &(CurrentWord.c_str())[2]);
			a = strtoll(buff, NULL, 2);
			CurrentWord = std::to_string(a);
		}
		if (execute(fstLiteralOfInteger) || execute(fstOctLit) || execute(fstBinLit)) {
			int* t = new int;
			int* t2 = new int;
			int* t3 = new int;
			*t = 1;
			*t2 = 4;
			if (execute(fstLiteralOfInteger)) *t3 = 1;
			else if (execute(fstOctLit)) *t3 = 3;
			else *t3 = 4;
			std::string numberLi = "lI";
			std::string num = std::to_string(counterLiteral++);
			/*_itoa_s(counterTokenInProgram, numberLi, 10);*/
			IT::Entry newEntry = createStructId(stringToChar(numberLi + num), lexTable.size, t, t2, t3, (char*)stringToChar(CurrentWord));

			IT::Add(idTable, newEntry);
			Add(lexTable, createStructLexem(LEX_LITERAL, numberOfstring, counterID++));
			//for (int i = 0; i < idTable.size; i++)
			//{
			//	if (!strcmp(newEntry.id, idTable.table[i].id)) 	Add(lexTable, createStructLexem(LEX_LITERAL, numberOfstring, counterID++));
			//}
			counterTokenInProgram++;
			delete t;
			delete t2;
			delete t3;
			return;
		}
		if (execute(fstLiteralOfString)) {
			int size = strlen(text);
			if (size > 257) throw ERROR_THROW(204);
			int* t = new int;
			int* t2 = new int;
			int* t3 = new int;
			 *t = 2;
			*t2 = 4;
			*t3 = 2;
			std::string numberLi = "lS";
			std::string num = std::to_string(counterLiteral++);

			/*_itoa_s(counterTokenInProgram, numberLi, 10);*/
			IT::Entry newEntry = createStructId(stringToChar(numberLi + num), lexTable.size, t, t2, t3, (char*)stringToChar(CurrentWord));
			newEntry.littype = (IT::LITERALTYPE)2;
			//IT::Add(idTable, newEntry);
			for (int i = 0; i < idTable.size; i++)
			{
				if (!strcmp(newEntry.id, idTable.table[i].id)) 	Add(lexTable, createStructLexem(LEX_LITERAL, numberOfstring, counterID++));
			}
			counterTokenInProgram++;
			delete t;
			delete t2;
			delete t3;
			return;
		}
		if (execute(fstId)) {
			std::string name;
			for (int j = 0; j < 5; j++) {
				name.push_back(text[j]);
				if (j == strlen(text - 1))break;
			}
			IT::Entry* view = new IT::Entry;
			//*view = stackCall.top();
			int* type = new int;
			int* t3 = new int;
			*t3 = 2;
			IT::Entry newEntry = createStructId(stringToChar(name), counterID++, (int*)typeData, (int*)typeID, t3, NULL);
			//IT::Entry newEntry = createStructId(stringToChar(name), counterID++, (int*)typeData, (int*)typeID, t3, NULL, view);
			if ((typeID == 2)) {
				stackCall.push(newEntry);
			}
			//IT::Add(idTable, newEntry);

			bool idInTable = false;

			for (int i = 0; i < idTable.maxsize; i++)
			{
				if (!strcmp(newEntry.id, idTable.table[i].id) && (newEntry.view == idTable.table[i].view)) idInTable = true;
			}

			Add(lexTable, createStructLexem(LEX_ID, numberOfstring, idTable.size - 1));
			//return 0;
		}
		

	}

	void LAN::Confirm() //подтвердить слово
	{
		Inspect();
		switch (ExpectedWord) {
			case PossibleNextWords::Any: {
				if (CurrentWord == "string" || CurrentWord == "integer") {
					ExpectedWord = PossibleNextWords::Function;
					SetType();
				}
				else if (CurrentWord == "main") {
					ExpectedWord = PossibleNextWords::BlockBeginsOrSemicolon;
				}
				else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::Function: {
				if (isFunctionExpression())	ExpectedWord = PossibleNextWords::FunctionIdentifier;
				else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::LeftBracketAfterFunction: {
				if (isLeftBracket())	ExpectedWord = PossibleNextWords::ParamTypeOrRightBracket;
				else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::ParamTypeOrRightBracket: {
				if (isRightBracket()) {
					ExpectedWord = PossibleNextWords::BlockBeginsOrSemicolon;
				}
				else if (isIntegerOrString()) {
					ExpectedWord = PossibleNextWords::ParamName;
					SetType();
				}
				else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::ParamCommaOrRightBracket: {
				if (isComma())	ExpectedWord = PossibleNextWords::ParamType;
				else if (isRightBracket()) ExpectedWord = PossibleNextWords::BlockBeginsOrSemicolon;
				//else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::ParamType: {
				if (isIntegerOrString()) {
					ExpectedWord = PossibleNextWords::ParamName;
					SetType();
				}
				//else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::BlockBeginsOrSemicolon: {
				if (isLeftBrace())	ExpectedWord = PossibleNextWords::InBlockAny;
				//else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::InBlockAny: {
				if (isDeclareExpression())		ExpectedWord = PossibleNextWords::InBlockType;
				else if (isIdentifierName())	ExpectedWord = PossibleNextWords::InBlockAssignment;
				else if (isReturnExpression())	ExpectedWord = PossibleNextWords::InBlockIdentifierOrLiteral;
				else if (isRightBrace())		ExpectedWord = PossibleNextWords::EndBlockSemicolon;
				else if (isPrintExpression())	ExpectedWord = PossibleNextWords::InBlockExpression;
				//else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::InBlockType: {
				if (isIntegerOrString()) {
					ExpectedWord = PossibleNextWords::InBlockIdentifierOrFunction;
					SetType();
				}
				//else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::InBlockIdentifierOrFunction: {
				if (isIdentifierName()) {
					IT::Entry entry;
					strncpy_s(entry.id, CurrentWord.c_str(), ID_MAXSIZE);
					entry.idtype = IT::V;
					if (Type::String == TypeIsStringOrInteger)	entry.iddatatype = IT::STR;
						else if (Type::Integer == TypeIsStringOrInteger) entry.iddatatype = IT::INT;
					entry.idxfirstLE = lexTable.size - 1;
					IT::Add(idTable, entry);
					ExpectedWord = PossibleNextWords::InBlockSemicolon;
				}
				else if (isFunctionExpression()) {
					ExpectedWord = PossibleNextWords::InBlockExpression;
				}
				//else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::InBlockSemicolon: {
				if (isSemicolon())	ExpectedWord = PossibleNextWords::InBlockAny;
				else ExpectedWord = PossibleNextWords::InBlockExpression;
				break;
			}
			case PossibleNextWords::InBlockAssignment: {
				if (isAssignment())	ExpectedWord = PossibleNextWords::InBlockExpression;
				//else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::InBlockExpression: {
				if (isSemicolon())	ExpectedWord = PossibleNextWords::InBlockAny;
				else ExpectedWord = PossibleNextWords::InBlockExpression;
				break;
			}
			case PossibleNextWords::InBlockIdentifierOrLiteral: {
				if (isIdentifierName() || isDigitLiteral() || isStringLiteral()) ExpectedWord = PossibleNextWords::InBlockSemicolon;
				//else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::EndBlockSemicolon: {
				if (isSemicolon())	ExpectedWord = PossibleNextWords::Any;
				//else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::ParamName: {
				if (isIdentifierName()) {
					IT::Entry entry;
					strncpy_s(entry.id, CurrentWord.c_str(), ID_MAXSIZE);
					entry.idtype = IT::P;
					if (Type::String == TypeIsStringOrInteger)	entry.iddatatype = IT::STR;
						else if (Type::Integer == TypeIsStringOrInteger)	entry.iddatatype = IT::INT;
					entry.idxfirstLE = lexTable.size - 1;
					IT::Add(idTable, entry);
					ExpectedWord = PossibleNextWords::ParamCommaOrRightBracket;
				}
				//else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			case PossibleNextWords::FunctionIdentifier: {
				if (isIdentifierName()) {//добавление в таблицу идентификаторов
					IT::Entry entry;
					strncpy_s(entry.id, CurrentWord.c_str(), ID_MAXSIZE);
					entry.idtype = IT::F;//функция
					if (TypeIsStringOrInteger == Type::String) {
						entry.iddatatype = IT::STR;//string
					}
					else if (TypeIsStringOrInteger == Type::Integer) {
						entry.iddatatype = IT::INT;//integer
					}
					entry.idxfirstLE = lexTable.size - 1;
					IT::Add(idTable, entry);
					ExpectedWord = PossibleNextWords::LeftBracketAfterFunction;
				}
				else throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
				break;
			}
			default: {
				throw ERROR_THROW_IN(3, currentLine, currentPosition - CurrentWord.size());
			}
		}
		CurrentWord.clear();
	}

	bool LAN::isFunctionExpression() {
		if (WordStates == State::StringLiteral) 	return false;
		FST::FST fst(CurrentWord.c_str(), 9, FST_FUNCTION);
		if (FST::execute(fst)) 	return true;
		return false;
	}
	bool LAN::isDeclareExpression() {
		if (WordStates == State::StringLiteral)	return false;
		FST::FST fst(CurrentWord.c_str(), 8, FST_DECLARE);
		if (FST::execute(fst)) 	return true;
		return false;
	}
	bool LAN::isMainExpression() {
		if (WordStates == State::StringLiteral)	return false;
		FST::FST fst(CurrentWord.c_str(), 5, FST_MAIN);
		if (FST::execute(fst)) 	return true;
		return false;
	}
	bool LAN::isPrintExpression() {
		if (WordStates == State::StringLiteral)	return false;
		FST::FST fst(CurrentWord.c_str(), 6, FST_PRINT);
		if (FST::execute(fst)) 	return true;
		return false;
	}
	bool LAN::isReturnExpression() {
		if (WordStates == State::StringLiteral)	return false;
		FST::FST fst(CurrentWord.c_str(), 7, FST_RETURN);
		if (FST::execute(fst)) 	return true;
		return false;
	}

	bool LAN::isIntegerOrString()
	{
		return (CurrentWord == "string" || CurrentWord == "integer");
	}
	bool LAN::isStringLiteral() {
		return (WordStates == State::StringLiteral);
	}
	bool LAN::isDigitLiteral() {
		if (CurrentWord.empty()) return false;
		for (int i = 0; i < CurrentWord.size(); i++)
			if (!isdigit(CurrentWord[i]))	return false;
		return true;
	}
	bool LAN::isPredefinedWord() { //integer, string, function, declare, main, print, return - true
		if (WordStates == State::StringLiteral) 		return false;
		if (CurrentWord == "integer" || CurrentWord == "string" || CurrentWord == "function" || CurrentWord == "declare" || CurrentWord == "main" || CurrentWord == "print" || CurrentWord == "return") return true;
		return false;
	}
	bool LAN::isIdentifierName() {
		if (CurrentWord.empty())	return false;
		if (isStringLiteral())	return false;
		if (isPredefinedWord())	return false;
		for (size_t i = 0; i < CurrentWord.size(); ++i) {
			if (!islower(CurrentWord[i])) {
				return false;
			}
		}
		return true;
	}

	bool LAN::iPlusMinusSlashStar() { //+ - / *
		if (WordStates == State::StringLiteral) 	return false;
		if (CurrentWord == "+" || CurrentWord == "-" || CurrentWord == "*" || CurrentWord == "/") return true;
		return false;
	}
	bool LAN::isLeftBracket() {
		if (WordStates == State::StringLiteral)		return false;
		if (CurrentWord == "(")	return true;
		return false;
	}
	bool LAN::isRightBracket() {
		if (WordStates == State::StringLiteral)			return false;
		if (CurrentWord == ")")	return true;
		return false;
	}
	bool LAN::isLeftBrace() {
		if (WordStates == State::StringLiteral)		return false;
		if (CurrentWord == "{")	return true;
		return false;
	}
	bool LAN::isRightBrace() {
		if (WordStates == State::StringLiteral)		return false;
		if (CurrentWord == "}")	return true;
		return false;
	}
	bool LAN::isSemicolon() {
		if (WordStates == State::StringLiteral)		return false;
		if (CurrentWord == ";")	return true;
		return false;
	}
	bool LAN::isComma() {
		if (WordStates == State::StringLiteral)	return false;
		if (CurrentWord == ",")	return true;
		return false;
	}
	bool LAN::isAssignment() {// =
		if (WordStates == State::StringLiteral) 		return false;
		if (CurrentWord == "=") 	return true;
		return false;
	}

	std::string LAN::IntToString(int par) {
		char buffer[128];
		_itoa_s(par, buffer, 10);
		std::string result = buffer;
		int pr = 3 - result.size();
		if (pr > 0) {
			for (int i = 0; i < pr; ++i)
				result.insert(result.begin(), '0');
		}
		return result;
	}
	LAN::LAN(Log::LOG logfile) {
		logFile = logfile;
		lexTable = LT::Create(LT_MAXSIZE);
		idTable = IT::Create(TI_MAXSIZE);
		WordStates = State::Token;				//состояние слова - лексема или из языка
		ExpectedWord = PossibleNextWords::Any;	//что ожидает
	}
	LAN::~LAN() {
		/*LT::Delete(lexTable);
		IT::Delete(idTable);*/
	}

	void LAN::Print()
	{
		const char* temp;
		std::ofstream fout("Lex.txt");
		std::ofstream lt("LT.txt");
		std::ofstream it("IT.txt");
		//fout << lexTable.LexemText << std::endl << std::endl;
		
		int currentLineIndex = 1;
		std::string currentLine;
		for (int i = 0; i < lexTable.size;) {
			char currentLexema;
			currentLexema = lexTable.table[i].lexema;
			if (currentLineIndex == lexTable.table[i].sn) { //номер строки равен номеру строки в таблице лексем 
				currentLine += currentLexema;
			}
			else {
				fout << (char*)IntToString(currentLineIndex).c_str() << " " << currentLine.c_str() << "\n";
				//Log::WriteLine(logFile, (char*)IntToString(currentLineIndex).c_str(), " ", currentLine.c_str(), "");
				currentLine = "";
				currentLineIndex++;
				continue;
			}
			i++;
		}
		fout << (char*)IntToString(currentLineIndex).c_str() << " " << currentLine.c_str() << "\n";
        lt << std::setw(3) << "№" << std::setw(10) << "Лексема" << std::setw(15) << "Номер строки" << std::setw(10) << "IDxTi" << std::endl;
		//Log::WriteLine(logFile, (char*)IntToString(currentLineIndex).c_str(), " ", currentLine.c_str(), "");
		lt << "---------------------------------------" << std::endl;
		for (int i = 0; i < lexTable.size; i++)
		{
			lt << std::setw(3) << i << std::setw(10) << lexTable.table[i].lexema << std::setw(15)
				<< lexTable.table[i].sn << std::setw(10) << lexTable.table[i].idxTI << std::endl;
			lt << "---------------------------------------" << std::endl;
		}
		for (int i = 0; i < idTable.size; i++)
		{
			temp = (const char*)lexTable.table[i].lexema;
			it << std::setw(3) << i << ". Идентификатор: " << std::setw(6) << idTable.table[i].id << ". Тип: " << std::setw(3) << idTable.table[i].idtype << ". Значение: " << "Тип переменной :" << idTable.table[i].iddatatype;
			if (idTable.table[i].view)it << ". Область видимости: " << std::setw(6) << idTable.table[i].view->id;
			else it << "                           ";
			it << ". Номер первого вхождения: " << std::setw(2) << idTable.table[i].idxfirstLE << "  . Тип литерала: " << idTable.table[i].littype << std::endl;
		}

	}
	void LAN::WriteLexTable() {
		Log::WriteLine(logFile, (char*)"--------- Таблица лексем --------------------------------", "");
		int currentLineIndex = 1;
		std::string currentLine;
		for (int i = 0; i < lexTable.size;) {
			char currentLexema;
			currentLexema = lexTable.table[i].lexema;
			if (currentLineIndex == lexTable.table[i].sn) { //номер строки равен номеру строки в таблице лексем 
				currentLine += currentLexema;
			}
			else {
				Log::WriteLine(logFile, (char*)IntToString(currentLineIndex).c_str(), " ", currentLine.c_str(), "");
				currentLine = "";
				currentLineIndex++;
				continue;
			}
			i++;
		}
		Log::WriteLine(logFile, (char*)IntToString(currentLineIndex).c_str(), " ", currentLine.c_str(), "");
	}
	void LAN::WriteIDTable() {
		Log::WriteLine(logFile, (char*)"--------- Таблица идентификаторов -----------------------", "");
		for (int i = 0; i < idTable.size; i++)
			Log::WriteLine(logFile, idTable.table[i].idtype == IT::F ? (char*)"function" : idTable.table[i].idtype == IT::P ? (char*)"param\t" : (char*)"variable", "  | ", idTable.table[i].iddatatype == IT::STR ? "string " : "integer", " | ", idTable.table[i].id, "  \t| ", IntToString(idTable.table[i].idxfirstLE).c_str(), "");
	}
}