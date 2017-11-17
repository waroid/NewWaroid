/*
 * GRCCoreUtil.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: mirime
 */

#include "GRCCoreUtil.h"

#include <sys/time.h>
#include <unistd.h>

unsigned int GRCCoreUtil::getTickCount()
{
	timeval tick;
	gettimeofday(&tick, 0);

	return tick.tv_sec * 1000 + tick.tv_usec / 1000;
}

void GRCCoreUtil::sleep(double seconds)
{
	usleep(static_cast<__useconds_t>(seconds * 1000000));
}
