/*
 * GRCCoreUtil.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: mirime
 */

#include "GRCCoreUtil.h"

#include <sys/time.h>

unsigned int GRCCoreUtil::getTickCount()
{
	timeval tick;
	gettimeofday(&tick, 0);

	return tick.tv_sec * 1000 + tick.tv_usec / 1000;
}
