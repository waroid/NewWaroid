/*
 * GRCLogger.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef GRCLOGGER_H_
#define GRCLOGGER_H_

#include "GRCString.h"

class GRCLogger
{
public:
	static bool isDev()
	{
		return s_dev;
	}
	static void setDev(bool dev)
	{
		s_dev = dev;
	}

	static void write(int level, GRCCSTR format, ...);

private:
	static bool s_dev;
};

#endif /* GRCLOGGER_H_ */
