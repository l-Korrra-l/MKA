#include "ASMGenerator.h"
#include "LAN.h"
#include "LT.h"
#include <fstream>
#include <iomanip>
using namespace std;

namespace ASMGenerator {
	ofstream asmFile("Generator/asm.asm");

	void AddSystemInfo() {
		asmFile << ASM_START_BLOCK;
		asmFile << ASM_LIBS_BLOCK;
		asmFile << endl << "includelib lib.lib" << endl;
	}

	void AddFuncInfo(Lan::LAN table) {
		asmFile << "\n\nExitProcess PROTO : DWORD\n";
		asmFile << "outint PROTO : DWORD\n";
		asmFile << "outstr PROTO : DWORD\n";
		asmFile << ASM_STACK_BLOCK;
	}

	void AddConstInfo(Lan::LAN table) {
		asmFile << ASM_CONST_BLOCK;
		asmFile << "\tdivisionByZero db 'ERROR: DIVIDE BY ZERO', 0" << endl;
		asmFile << "\tbelowZeroNum db 'ERROR: NUM IS BELOVER THAN ZERO', 0" << endl;
		asmFile << ASM_DATA_BLOCK;
		for (int i = 0; i <table.idTable.size; i++) {
			//if (table.lexTable.table[i].lexema == LEX_DECLARE && (table.lexTable.table[i + 1].lexema == LEX_STRING || table.lexTable.table[i + 1].lexema == LEX_INTEGER)) {
			//	if (table.idTable.table[table.lexTable.table[i + 2].idxTI+1].iddatatype == 1) {
			//	asmFile << "\t" << table.idTable.table[table.lexTable.table[i + 2].idxTI+1].id << " SDWORD ?" << endl;
			//}
			//	if (table.idTable.table[table.lexTable.table[i + 2].idxTI+1].iddatatype== 2) {
			//	asmFile << "\t" << table.idTable.table[table.lexTable.table[i].idxTI+1].id << " BYTE ? , 0" << endl;
			//}
			if (table.idTable.table[i].idtype== IT::P|| table.idTable.table[i].idtype == IT::V)
			{
				if (table.idTable.table[i].iddatatype == IT::STR)
					asmFile << "\t" << table.idTable.table[i].id << " BYTE ? , 0" << endl;
				if (table.idTable.table[i].iddatatype == IT::INT)
					asmFile << "\t" << table.idTable.table[i].id << " SDWORD ?" << endl;
			}
		}
	}



	void AddCodeBlock() {
		asmFile << "\n.code" << endl;
	}

	int equ = 0;
	bool flag = false;
	bool isIf = false;
	bool isElse = false;
	bool isPol = false;
	bool hasDivide = false;
	bool callFunc = false;
	int funcPos = 0;

	void AddMainProc(Lan::LAN table) {
		string name;
		string functionName;
		string nameOfEqu;
		string name1;
		bool isMain = false;
		bool isFunc = false;
		int finish = 0;
		int  deltaIf = 0;
		int position = 0;
		for (int i = 0; i < table.lexTable.size; i++) {
			switch (table.lexTable.table[i].lexema)
			{
			case LEX_MAIN:
				asmFile << "\nmain PROC" << endl;
				isMain = true;
				functionName = "main";
				break;
			case LEX_FUNCTION:
				functionName = table.idTable.table[table.lexTable.table[i].idxTI+1].id;
				asmFile << "\n"<< functionName<<" PROC  ";
				while (table.lexTable.table[i].lexema != LEX_CLOSEBRACKET)
				{
					int count = 0;
					i++;
					if ((table.lexTable.table[i-1].lexema == LEX_INTEGER || table.lexTable.table[i-1].lexema == LEX_STRING) && table.lexTable.table[i].lexema == LEX_ID)
					{
						asmFile << table.idTable.table[table.lexTable.table[i].idxTI + 1].id;
						if (table.idTable.table[table.lexTable.table[i].idxTI + 1].iddatatype == IT::STR)
							asmFile << " : BYTE ";
						else asmFile<< " : SDWORD ";
						count++;
						  
					}
					if (table.lexTable.table[i].lexema == LEX_COMMA)
					{
						asmFile << ", "; i++;
					}
				}
				isFunc = true;
				break;
			case LEX_PRINT:
				if (table.idTable.table[table.lexTable.table[i + 1].idxTI+1].littype == 1) {
					asmFile << "\tpush " << name << table.idTable.table[table.lexTable.table[i + 2].idxTI+1].id;
					asmFile << "\n\tcall outint\n";
					asmFile << "\n\tpop " << name << table.idTable.table[table.lexTable.table[i + 2].idxTI+1].id << endl;
				}
				if (table.idTable.table[table.lexTable.table[i + 1].idxTI+1].littype == 2) {
					asmFile << "\tpush offset " << table.idTable.table[table.lexTable.table[i + 2].idxTI+1].id;
					asmFile << "\n\tcall outstr\n";
				}
				break;
			case LEX_RETURN:
				if (hasDivide) {
					asmFile << "\tpop eax" << endl;
					asmFile << "\tret" << endl;
					asmFile << "\tcall exitProcess" << endl;
					asmFile << "\tfinish:" << endl;
					asmFile << "\tpush offset divisionByZero" << endl;
					asmFile << "\tcall outstr" << endl;
					asmFile << "\tcall exitProcess" << endl;
				}
				asmFile << "\tret" << endl;
				asmFile << "\tbelow:" << endl;
				asmFile << "\tpush offset belowZeroNum" << endl;
				asmFile << "\tcall outstr" << endl;
				asmFile << "\tcall exitProcess" << endl;
				hasDivide = false;
				if (!isFunc) asmFile << "glob_" << functionName << " ENDP" << endl;
				break;
			case LEX_ASSIGNMENT:

				if (table.lexTable.table[i + 1].lexema == LEX_ID && table.idTable.table[table.lexTable.table[i + 1].idxTI+1].littype == -1) {
					functionName = table.idTable.table[table.lexTable.table[i + 1].idxTI+1].id;
				}
				equ = i;

				name1 = table.idTable.table[table.lexTable.table[equ - 1].idxTI+1].id;
				while (table.lexTable.table[i].lexema != LEX_SEMICOLON) {
					if (table.lexTable.table[i].lexema == LEX_ID && table.lexTable.table[i + 1].lexema != LEX_COMMA && table.lexTable.table[i - 1].lexema != LEX_COMMA && table.lexTable.table[i + 1].lexema != LEX_OPENBRACKET) {
						isPol = false;

						asmFile << "\tpush " << name + table.idTable.table[table.lexTable.table[i].idxTI+1].id << endl;
					}
					if (table.lexTable.table[i].lexema == LEX_LITERAL && table.lexTable.table[i + 1].lexema != LEX_COMMA && table.lexTable.table[i - 1].lexema != LEX_COMMA && table.lexTable.table[i + 1].lexema != LEX_OPENBRACKET) {
						isPol = false;

						asmFile << "\tpush " << name + table.idTable.table[table.lexTable.table[i].idxTI+1].id << endl;
					}

					if (table.lexTable.table[i].lexema == LEX_ID && table.lexTable.table[i + 1].lexema == LEX_OPENBRACKET) {
						funcPos = i;
						while (table.lexTable.table[i + 1].lexema != LEX_CLOSEBRACKET) {
							if (table.lexTable.table[i + 1].lexema == LEX_LITERAL) {
								asmFile << "\tpush " << table.idTable.table[table.lexTable.table[i + 1].idxTI+1].id << endl;
							}
							i++;
						}
					}

					if ((table.lexTable.table[i - 1].lexema == LEX_ID || table.lexTable.table[i - 1].lexema == LEX_LITERAL) && table.lexTable.table[i].lexema == LEX_CLOSEBRACKET) {
						position = i;
						while (table.lexTable.table[i - 1].lexema != LEX_OPENBRACKET)
						{
							if (table.lexTable.table[i - 1].lexema == LEX_ID) {

								
								asmFile << "\tpush " << name + table.idTable.table[table.lexTable.table[i - 1].idxTI+1].id << endl;
								name.clear();
							}
							if (table.lexTable.table[i - 1].lexema == LEX_LITERAL) {

								asmFile << "\tpush " << table.idTable.table[table.lexTable.table[i - 1].idxTI+1].id << endl;
							}
							i--;
						}
						i = position;

						asmFile << "\tcall glob_" << functionName << endl;
						asmFile << "\tmov " << name + table.idTable.table[table.lexTable.table[equ - 1].idxTI+1].id << ", eax" << endl;
						name.clear();
					}
					if (table.lexTable.table[i - 1].lexema == LEX_ID && table.lexTable.table[i + 1].lexema == LEX_LITERAL && table.lexTable.table[i + 2].lexema == LEX_SEMICOLON) {
						isPol = false;
						if (table.idTable.table[table.lexTable.table[i - 1].idxTI+1].iddatatype == 1) {
							asmFile << "\tmov " << name + table.idTable.table[table.lexTable.table[i - 1].idxTI+1].id << ", " << table.idTable.table[table.lexTable.table[i + 1].idxTI+1].value.vint << endl;
						}
					}
					if (table.lexTable.table[i].lexema == LEX_PLUS) {
						isPol = true;
						asmFile << "\tpop ebx\n\tpop eax" << endl;
						asmFile << "\tadd eax, ebx" << endl;
						asmFile << "\tpush eax" << endl;
					}
					if (table.lexTable.table[i].lexema == LEX_MINUS) {
						isPol = true;
						asmFile << "\tpop ebx\n\tpop eax" << endl;
						asmFile << "\tsub eax, ebx" << endl;
						asmFile << "\tpush eax" << endl;
					}
					if (table.lexTable.table[i].lexema == LEX_STAR) {
						isPol = true;
						asmFile << "\tpop ebx\n\tpop eax" << endl;
						asmFile << "\timul ebx" << endl;
						asmFile << "\tpush eax" << endl;
					}
					if (table.lexTable.table[i].lexema == LEX_SLASH) {
						isPol = true;
						hasDivide = true;
						asmFile << "\tpop ebx" << endl;
						asmFile << "\tmov edx, 0 " << endl;
						asmFile << "\tpop eax" << endl;
						asmFile << "\tcmp ebx, 0" << endl;
						asmFile << "\tje finish" << endl;
						asmFile << "\tidiv ebx" << endl;
						asmFile << "\tpush eax" << endl;
					}

					i++;
				}
				if (isPol) {
					asmFile << "\tpop eax" << endl;
					asmFile << "\tcmp eax, 0" << endl;
					asmFile << "\tjl below" << endl;
					asmFile << "\tmov " << nameOfEqu + name1 << ", eax" << endl;
				}
				nameOfEqu.clear();
				break;
			case LEX_IF:
				isIf = true;
				if (table.lexTable.table[i + 3].lexema == LEX_MORE) {
					asmFile << "\tpush " << name + table.idTable.table[table.lexTable.table[i + 2].idxTI+1].id << endl;
					asmFile << "\tpush " << name + table.idTable.table[table.lexTable.table[i + 4].idxTI+1].id << endl;
					asmFile << "\tpop ecx" << endl;
					asmFile << "\tpop edx" << endl;
					asmFile << "\tcmp edx, ecx" << endl;
					asmFile << "\tjg trueResult" << endl;
					asmFile << "\tjl falseResult" << endl;
				}
				if (table.lexTable.table[i + 3].lexema == LEX_LESS) {
					asmFile << "\tpush " << name + table.idTable.table[table.lexTable.table[i + 2].idxTI+1].id << endl;
					asmFile << "\tpush " << name + table.idTable.table[table.lexTable.table[i + 4].idxTI+1].id << endl;
					asmFile << "\tpop ecx" << endl;
					asmFile << "\tpop edx" << endl;
					asmFile << "\tcmp ecx, edx" << endl;
					asmFile << "\tjg trueResult" << endl;
					asmFile << "\tjl falseResult" << endl;
				}
				if (table.lexTable.table[i + 3].lexema == LEX_EQUAL) {
					asmFile << "\tpush " << table.idTable.table[table.lexTable.table[i + 2].idxTI+1].id << endl;
					asmFile << "\tpush " << table.idTable.table[table.lexTable.table[i + 4].idxTI+1].id << endl;
					asmFile << "\tpop ecx" << endl;
					asmFile << "\tpop edx" << endl;
					asmFile << "\tcmp ecx, edx" << endl;
					asmFile << "\tjg trueResult" << endl;
					asmFile << "\tjl falseResult" << endl;
				}
				asmFile << "\ttrueResult:" << endl;
				break;
				i = deltaIf;
			case LEX_ELSE:
				isIf = false;
				isElse = true;
				asmFile << "\tfalseResult:" << endl;
				break;
			case LEX_ENDBLOCK:
				if (isIf) asmFile << "\tjmp code" << endl;
				asmFile << "\tleave" << endl;
				if (isElse) asmFile << "\tcode:" << endl;
				break;
			default:
				break;
			}
		}
	}

	void AddEndProc() {
		asmFile << ASM_MAIN_END_BLOCK << endl;
	}

	void Generate(Lan::LAN table) {
		AddSystemInfo();
		AddFuncInfo(table);
		AddConstInfo(table);
		AddCodeBlock();
		AddMainProc(table);
		AddEndProc();

	}
}