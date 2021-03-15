#include "In.h"
#include "Error.h"
#include "FST.h"
#include <fstream>
#include <string>
namespace In
{
	IN In::getin(wchar_t infile[])
	{
		IN newIn{0,1,0};
		newIn.text = new unsigned char[SIZE];
		unsigned char* newInPointer = newIn.text;
		unsigned char symbol;
		bool quotesFlag = 0;

		std::ifstream fileIn(infile);
		if (!fileIn.is_open()) throw ERROR_THROW(110);

		int currentColumn = 1;
		for (int i = 0; !fileIn.eof(); i++)
		{
			symbol = fileIn.get();
			if (fileIn.eof()) {
				*newInPointer = IN_CODE_ENDL;
				newInPointer++;
				*newInPointer = IN_CODE_N;
				break;
			}

			if (symbol == IN_CODE_ENDL) {
				newIn.lines++;
				currentColumn = 1;
			}
			else
				currentColumn++;

			switch (newIn.code[symbol])
			{
			case IN::F: {
				if (quotesFlag) {
					*newInPointer = symbol;
					newInPointer++;
					if (symbol != IN_CODE_ENDL) newIn.size++;
					break;
				}
				throw ERROR_THROW_IN(111, newIn.lines, currentColumn);
			}
			case IN::I: {
				newIn.ignor++;
				break;
			}
			case IN::T: {
				if (symbol == QUOTE) quotesFlag = !quotesFlag;
				if (quotesFlag == false)
				{
					if (!delSpace(symbol, *(newInPointer - 1)))
					{
						*newInPointer = symbol;
						newInPointer++;
						
	
					}
					else {  *(newInPointer-1) = symbol;}
				}
				else
				{
					*newInPointer = symbol;
					newInPointer++;
				}
				if (symbol != IN_CODE_ENDL) newIn.size++;
				break;
			}
			case IN::S: {
				if (quotesFlag == false)
				{
					if (!delSpace(*(newInPointer - 1), symbol))
					{
						*newInPointer = symbol;
						newInPointer++;
						newIn.size++;
					}
				}
				else
				{
					*newInPointer = symbol;
					newInPointer++;
				}
				break;
			}
			case IN::R: {
				if (symbol == QUOTE) quotesFlag = !quotesFlag;
				if (quotesFlag == 0) throw ERROR_THROW_IN(4, newIn.lines, currentColumn);
				*newInPointer = symbol;
				newInPointer++;
				if (symbol != IN_CODE_ENDL) newIn.size++;
				break;
			}
			default: {
				*newInPointer = newIn.code[symbol];
				newInPointer++;
				newIn.size++;
				break;
			}
			}
		}
		/*newInPointer = 0;*/
		fileIn.close();
		return newIn;
	}
	char* stringToChar(std::string str) {
		char* massChar = new char[255];
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == '\\' && str[i + 1] == '"') continue;
			massChar[i] = str[i];
		}
		massChar[str.length()] = '\0';
		return massChar;
	}
	void IN::change()
	{
		char* t;
		int initPos,finPos;
		char buff[10];
		int a = 0;
		int j;
		std::string CurrentWord;

		for (int i = 0; i < size; i++)
		{
			if (text[i] == '0')
			{
				j = i;
				int wSize = 0;
				initPos = i;
				while (isdigit(text[j]) || text[j] == 'b' || text[j] == 'x')
				{
					CurrentWord += text[j];
					finPos = j;
					j++;
				}
				t=stringToChar(CurrentWord);
				FST::FST fstBinLit(t, 4,
					FST::NODE(1, FST::RELATION('0', 1)),
					FST::NODE(1, FST::RELATION('b', 2)),
					FST::NODE(4, FST::RELATION('0', 2), FST::RELATION('1', 2),
						FST::RELATION('0', 3), FST::RELATION('1', 3)),
					FST::NODE()
				);
				if (execute(fstBinLit))// && text[i + 1] == 'b')
				{
					strcpy_s(buff, &(CurrentWord.c_str())[2]);
					a = strtoll(buff, NULL, 2);
					CurrentWord = std::to_string(a);
					t = stringToChar(CurrentWord);
					wSize = CurrentWord.length();
					for (int l = i, n = 0; l < j; l++, n++)
					{
						if (wSize > 0)
						{
							text[l] = t[n];
							wSize--;
						}
						else text[l] = ' ';
					}
				}
			}
		}
	}

	void IN::EditText() {
		unsigned char* Pointer = text;
		for (int i = 0; text[i] != '\0'; i++)
		{
			if (delSpace(text[i], text[i + 1]) || delSpace(text[i + 1], text[i]))
			{
				size--;  continue;
			}
			*Pointer = text[i]; Pointer++;
		}
		*Pointer = IN_CODE_N;
	};
}