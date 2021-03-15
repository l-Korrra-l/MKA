#include "FST.h"
#include <iostream>

namespace FST {
	RELATION::RELATION(char c, short nn) {
		symbol = c;
		nnode = nn;
	};
	NODE::NODE() {
		n_relation = 0;
		RELATION* relations = NULL;
	};
	NODE::NODE(short n, RELATION rel, ...) {
		n_relation = n;
		RELATION* p = &rel;
		relations = new RELATION[n];
		for (short i = 0; i < n; i++)
			relations[i] = p[i];
	};
	FST::FST(const char* s, short ns, NODE n, ...)
	{
		string = s;
		nstates = ns;
		nodes = new NODE[ns];
		NODE* p = &n;
		for (int k = 0; k < ns; k++) nodes[k] = p[k];
		rstates = new short[nstates];
		memset(rstates, 0xff, sizeof(short) * nstates);
		rstates[0] = 0;
		position = -1;
	};
	bool step(FST& fst, short*& rstates)
	{
		bool rc = false;
		std::swap(rstates, fst.rstates);
		for (short i = 0; i < fst.nstates; i++) {
			if (rstates[i] == fst.position)
				for (short j = 0; j < fst.nodes[i].n_relation; j++) {
					if (fst.nodes[i].relations[j].symbol == fst.string[fst.position])
					{
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
					}
				}
		}
		return rc;
	};
	bool execute(FST& fst)
	{
		short* rstates = new short[fst.nstates];
		memset(rstates, 0xff, sizeof(short) * fst.nstates);
		short lstring = strlen(fst.string);
		bool rc = true;
		for (short i = 0; i < lstring && rc; i++) {
			fst.position++;
			rc = step(fst, rstates);
		}
		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	}

	bool step(FST& fst, short*& rstates, std::string str)
	{
		bool rc = false;
		std::swap(rstates, fst.rstates);
		for (short i = 0; i < fst.nstates; ++i)
			if (rstates[i] == fst.position)
			{
				for (short j = 0; j < fst.nodes[i].n_relation; ++j)
				{
					if (fst.nodes[i].relations[j].symbol == (unsigned char)str[fst.position])
					{
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
					}
				}
			}

		return rc;
	}

	bool execute(FST& fst, std::string str) {
		short* rstates = new short[fst.nstates];
		short lstring = (short)str.size();
		bool rc = true;

		memset(rstates, 0xff, sizeof(short) * fst.nstates);

		fst.position = -1;
		*fst.rstates = 0;

		for (short i = 0; i < lstring && rc; ++i) {
			fst.position++;
			rc = step(fst, rstates, str);
		}
		delete[] rstates;

		return rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc;
	}
};
