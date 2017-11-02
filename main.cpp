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
	if (argc < 5)
	{
		GRC_LOG("usage: %s <robot id> <robot type name> <game sever ip>  <dev 0 or 1>", argv[0]);
		return -1;
	}

	GRCLogger::setDev(atoi(argv[4]) == 1);

	GRC_LOG("[New Waroid] START");

	GRC_CHECK_FUNC_RETMINUS(initialize(), cleanup(0));

	int robotId = atoi(argv[1]);
	const char* robotTypeName = argv[2];
	const char* gameServerIp = argv[3];

	Manager::start(robotId, robotTypeName, gameServerIp);

	cleanup(0);

	GRC_LOG("[New Waroid] END");

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
	if (s != 0)
	{
		GRC_LOG("caught signal %d", s);
		exit(-1);
	}
}
