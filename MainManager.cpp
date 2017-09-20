/*
 * MainManager.cpp
 *
 *  Created on: 2015. 2. 13.
 *      Author: mirime
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include "core/Logger.h"

#include "MainManager.h"

namespace MAIN_MANAGER
{
	const int MAX_BUFFER_SIZE = 128;
	const int INVALID_SOCKET = -1;
}
using namespace MAIN_MANAGER;

MainManager::MainManager()
{
}

MainManager::~MainManager()
{
}

bool MainManager::start()
{
	return true;
}

void MainManager::stop()
{
}