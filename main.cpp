/*
 * main.cpp
 *
 *  Created on: 2015. 2. 12.
 *      Author: mirime
 */

#include <wiringPi.h>
#include <csignal>
#include <cstdlib>

#include "core/GRCCore.h"
#include "core/GRCLogger.h"
#include "Manager.h"

bool initialize();
void cleanup(int s);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		GRC_LOG("usage: %s <game server ip> <dev 0 or 1>", argv[0]);
		return -1;
	}

	GRCLogger::setDev(atoi(argv[1]) == 1);

	GRC_CHECK_FUNC_RETMINUS(initialize(), cleanup(0));

	int robotId = 0;
	int robotType = 1;
	const char* gameServerIp = "192.168.1.99";
	const char* controlBoardDevice = "/dev/ttyS0";

	GRC_CHECK_FUNC_RETMINUS(Manager::start(robotId, robotType, gameServerIp, controlBoardDevice), cleanup(0));

	cleanup(0);

	return 0;
}

bool initialize()
{
	signal(SIGINT, cleanup);
	signal(SIGTERM, cleanup);
	signal(SIGQUIT, cleanup);
	signal(SIGHUP, cleanup);

	GRC_CHECK_RETFALSE(wiringPiSetupGpio() != -1);
	GRC_DEV("setup gpio of wiringPi");

	GRC_LOG("initlaized.");

	return true;
}

void cleanup(int s)
{
	Manager::stop();
	GRC_DEV("caught signal %d", s);
	if (s != 0) exit(-1);
}
