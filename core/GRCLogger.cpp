/*
 * GRCLogger.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#include "GRCLogger.h"

#include <syslog.h>
#include <cstdarg>
#include <cstdio>

bool GRCLogger::s_dev = true;

void GRCLogger::write(int level, GRCCSTR format, ...)
{
	{
		va_list vl;
		va_start(vl, format);
		vsyslog(level | LOG_LOCAL0, format, vl);
		va_end(vl);
	}

	if (s_dev)
	{
		va_list vl;
		va_start(vl, format);
		vprintf(format, vl);
		printf("\n");
		va_end(vl);
	}

}

