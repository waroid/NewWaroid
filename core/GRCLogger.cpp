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

bool GRCLogger::s_dev = false;

void GRCLogger::write(int level, const char* format, ...)
{
	va_list vl;
	va_start(vl, format);
	vsyslog(level | LOG_LOCAL0, format, vl);

	if (s_dev)
	{
		vprintf(format, vl);
	}

	va_end(vl);
}

