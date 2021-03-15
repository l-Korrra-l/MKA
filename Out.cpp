#include "Out.h"
#include "Error.h"
#include "Parm.h"
#include <iostream>

namespace Out
{
	OUT getout(wchar_t outfile[]) {
		OUT newOut{};
		newOut.stream = new std::ofstream;
		newOut.stream->open(outfile);
		if (!newOut.stream->is_open())
			throw ERROR_THROW(112);
		wcscpy_s(newOut.outfile, outfile);
		return newOut;
	}

	void WriteOut(OUT newOut, In::IN in) {
		*newOut.stream << in.text;
	}

	void Close(OUT newOut) {
		if (newOut.stream != nullptr) {
			newOut.stream->close();
			delete newOut.stream;
		}
	}
};