/*
 * main.cpp
 *
 *  Created on: 2015. 2. 12.
 *      Author: mirime
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>

#include "core/Logger.h"
#include "MainManager.h"

MainManager mainManager;

bool initialize();
void cleanup(int s);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		GLOG("usage: %s <dev 0 or 1>", argv[0]);
		return -1;
	}
	
	Logger::setDev(atoi(argv[1]) == 1);

	
	if (initialize() == false)
	{
		GLOG("failed initialize()");
		cleanup(0);
		return -1;
	}

	mainManager.start();

	cleanup(0);

	return 0;
}

bool initialize()
{
	signal(SIGINT, cleanup);
	signal(SIGTERM, cleanup);
	signal(SIGQUIT, cleanup);
	signal(SIGHUP, cleanup);

	return true;
}

void cleanup(int s)
{
	mainManager.stop();
	GLOG("caught signal %d", s);
	if (s != 0) exit(-1);
}
