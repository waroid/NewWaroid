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

namespace GRC_LOGGER
{
	const int MAX_BUFFER_SIZE = 512;
}
using namespace GRC_LOGGER;

bool GRCLogger::s_dev = false;

void GRCLogger::write(const char* format, ...)
{
	char temp[MAX_BUFFER_SIZE] = {0};

	va_list vl;
	va_start(vl, format);
	int len = vsnprintf(temp, MAX_BUFFER_SIZE - 2, format, vl);
	va_end(vl);

	if (len > 0)
	{
		temp[len] = 0;
		syslog(LOG_USER, temp);
	}
}


