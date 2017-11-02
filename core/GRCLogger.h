/*
 * GRCLogger.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef GRCLOGGER_H_
#define GRCLOGGER_H_

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

	static void write(int level, const char* format, ...);

private:
	static bool s_dev;
};

#endif /* GRCLOGGER_H_ */
