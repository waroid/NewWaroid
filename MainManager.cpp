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
#include <pthread.h>
#include "core/Logger.h"

#include "MainManager.h"

namespace MAIN_MANAGER
{
	const int MAX_BUFFER_SIZE = 128;
	const int INVALID_SOCKET = -1;
}
using namespace MAIN_MANAGER;

MainManager::MainManager()
		: m_listenSocket(INVALID_SOCKET), m_ownerSocket(INVALID_SOCKET), m_networkThread(-1)
{
}

MainManager::~MainManager()
{
}

bool MainManager::start()
{
	GCHECK_RETFALSE(tcpListen());
	GLOG("listened tcp");

	pthread_join(m_networkThread, NULL);

	return true;
}

void MainManager::stop()
{
	if (m_networkThread >= 0)
	{
		if (pthread_cancel(m_networkThread) == 0)
		{
			pthread_join(m_networkThread, NULL);
		}
		GLOG("cancel network thread");
	}

	if (m_ownerSocket != INVALID_SOCKET)
	{
		close(m_ownerSocket);
		m_ownerSocket = INVALID_SOCKET;
		GLOG("close owner socket");
	}

	if (m_listenSocket != INVALID_SOCKET)
	{
		close(m_listenSocket);
		m_listenSocket = INVALID_SOCKET;
		GLOG("close listen socket");
	}
}

bool MainManager::tcpListen()
{
	m_listenSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
	GCHECK_RETFALSE(m_listenSocket != INVALID_SOCKET);

	int optval = 1;
	setsockopt(m_listenSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	struct sockaddr_in sockaddrIn;
	memset(&sockaddrIn, 0, sizeof(sockaddrIn));
	sockaddrIn.sin_family = AF_INET;
	sockaddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddrIn.sin_port = htons(5002);

	GCHECK_RETFALSE(bind(m_listenSocket, (struct sockaddr* ) &sockaddrIn, sizeof(sockaddrIn)) != -1);
	GCHECK_RETFALSE(listen(m_listenSocket, 3) != -1);

	GCHECK_RETFALSE(pthread_create(&m_networkThread, NULL, networkWorker, this)==0);

	return true;
}

void MainManager::tcpLoop()
{
	fd_set master_fds;
	FD_ZERO(&master_fds);
	FD_SET(m_listenSocket, &master_fds);
	int fd_max = m_listenSocket;
	fd_set read_fds;
	WAROIDROBOTCONTROL control;
	for (;;)
	{
		read_fds = master_fds;
		GCHECK_RETURN(select(fd_max + 1, &read_fds, 0, 0, 0) != -1);

		if (FD_ISSET(m_listenSocket, &read_fds))
		{
			struct sockaddr_in sockaddrIn;
			socklen_t socklen = sizeof(sockaddrIn);
			int s = accept(m_listenSocket, (struct sockaddr*) &sockaddrIn, &socklen);
			GLOG("new connection. socket=%d addr=%s", s, inet_ntoa(sockaddrIn.sin_addr));
			if (m_ownerSocket == INVALID_SOCKET)
			{
				m_ownerSocket = s;
				FD_SET(s, &master_fds);
				if (s > fd_max) fd_max = s;
			}
			else
			{
				GLOG("close connection. reason=EXIST_OWNER, socket=%d addr=%s", s, inet_ntoa(sockaddrIn.sin_addr));
				tcpSend(s, WAROIDROBOTCONTROLCOMMAND::ERROR_ACK, WAROIDROBOTCONTROLERROR::EXIST_OWNER);
				tcpDisconnect(s);
			}
		}

		if (m_ownerSocket != INVALID_SOCKET && FD_ISSET(m_ownerSocket, &read_fds))
		{
			int recvLen = recv(m_ownerSocket, &control, sizeof(control), 0);
			if (recvLen <= 0)
			{
				GLOG("disconnect. recv=%d err=%s(%d)", recvLen, strerror(errno), errno);
				FD_CLR(m_ownerSocket, &master_fds);
				tcpDisconnect(m_ownerSocket);
			}
			else if (recvLen == sizeof(control))
			{
				onProcess(control);
			}
			else
			{
				GLOG("invalid packet size. size=%d", recvLen);
				tcpDisconnect(m_ownerSocket);
			}

		}
	}
}

void MainManager::tcpSend(int socket, WAROIDROBOTCONTROLCOMMAND::ETYPE command, signed char value0, signed char value1)
{
	WAROIDROBOTCONTROL control;
	control.command = (signed char) command;
	control.value0 = value0;
	control.value1 = value1;
	send(socket, &control, sizeof(control), 0);
}

void MainManager::tcpSend(int socket, WAROIDROBOTCONTROLCOMMAND::ETYPE command, unsigned short value)
{
	WAROIDROBOTCONTROL control;
	control.command = (signed char) command;
	control.value = value;
	send(socket, &control, sizeof(control), 0);
}

void MainManager::tcpDisconnect(int socket)
{
	close(socket);
	if (m_ownerSocket == socket)
	{
		system("killall raspivid");
		m_ownerSocket = INVALID_SOCKET;
	}
}

void MainManager::onProcess(const WAROIDROBOTCONTROL& control)
{
	GDEV("received. command=%d value=%u(%d,%d)", control.command, control.value, control.value0, control.value1);
	switch (control.command)
	{
		case WAROIDROBOTCONTROLCOMMAND::CAMERA:
		{
			bool onoff = (control.value0 == 1);
			if (onoff)
			{
				int bitRate = 15000000;
				switch (control.value1)
				{
					case 1:
						bitRate = 8000000;
					break;
					case 2:
						bitRate = 4000000;
					break;
					case 3:
						bitRate = 2000000;
					break;
					case 4:
						bitRate = 1000000;
					break;
				}

				char systemCommand[256];
				sprintf(systemCommand, "raspivid -o - -t 0 -w 1280 -h 720 -fps 25 -hf -n -b %d  | nc -l -p 5001 &", bitRate);
				system(systemCommand);
				GLOG("open camera. system=%s", systemCommand);

			}
			else
			{
				system("killall raspivid");
				//system("killall nc");
				GLOG("close camera. system=killall raspivid");
			}
		}
		break;
	}
}

void* MainManager::networkWorker(void* param)
{
	GLOG("start network thread");
	MainManager* mainManager = (MainManager*) param;
	mainManager->tcpLoop();
	GLOG("stop network thread");

	return NULL;
}
