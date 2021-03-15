#pragma once
#include <string>
#include <vector>
#include "LT.h"
#include "LAN.h"

#define ASM_START_BLOCK                       ".586\n.model flat,stdcall\n"
#define ASM_LIBS_BLOCK                        "includelib libucrt.lib\nincludelib kernel32.lib\n"
#define ASM_CONST_BLOCK                       "\n.const\n"
#define ASM_STACK_BLOCK                       "\n.stack 4096\n"
#define ASM_DATA_BLOCK                        "\n.data\n"
#define ASM_MAIN_BLOCK                        "\nmain PROC\n"
#define ASM_MAIN_END_BLOCK                    "\nend main\n"


namespace ASMGenerator {
	std::string naming(Lan::LAN table, int pos);
	void AddSystemInfo();
	void AddConstInfo(Lan::LAN table);
	void AddFuncInfo(Lan::LAN table);
	void AddCodeBlock();

	void AddMainProc(Lan::LAN table);
	void AddEndProc();
	void Generate(Lan::LAN table);
}
