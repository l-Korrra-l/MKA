#include "Parm.h"
#include "Error.h"
#include <iostream>
#include <tchar.h>

namespace Parm
{
	PARM getparm(int argc, _TCHAR* argv[])
	{
		PARM newParm{};
		for (int i = 1; i < argc; i++)
			if (wcslen(argv[i]) > PARM_MAX_SIZE)
				throw ERROR_THROW(104);

		for (int i = 1, counterPARM_IN = 0; i <= argc; i++)
		{
			if (argc == 1)
				throw ERROR_THROW(100);
			wchar_t* parameter = argv[i];
			if (wcsstr(parameter, PARM_IN))
			{
				parameter += wcslen(PARM_IN);
				wcscpy_s(newParm.in, parameter);
				wcscpy_s(newParm.out, parameter);
				wcsncat_s(newParm.out, PARAM_OUT_DEFAULT_EXT, wcslen(PARAM_OUT_DEFAULT_EXT));
				wcscpy_s(newParm.log, parameter);
				wcsncat_s(newParm.log, PARM_LOG_DEFAULT_EXC, wcslen(PARM_LOG_DEFAULT_EXC));
				break;
			}
			else
				counterPARM_IN++;
			if (counterPARM_IN == (argc - 1))
				throw ERROR_THROW(100);

		}

		for (int i = 1; i < argc; i++) {
			wchar_t* parameter = argv[i];
			if (wcsstr(parameter, PARM_OUT))
			{
				parameter += wcslen(PARM_OUT);
				wcscpy_s(newParm.out, parameter);
			}
			if (wcsstr(parameter, PARM_LOG))
			{
				parameter += wcslen(PARM_LOG);
				wcscpy_s(newParm.log, parameter);
			}
		}
		return newParm;
	}
};