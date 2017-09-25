/*
 * MainManager.h
 *
 *  Created on: 2015. 2. 13.
 *      Author: mirime
 */

#ifndef MAINMANAGER_H_
#define MAINMANAGER_H_

#include "Global.h"

class MainManager
{
public:
	MainManager();
	virtual ~MainManager();

public:
	bool start();
	void stop();

private:
	bool tcpListen();
	void tcpLoop();
	void tcpSend(int socket, WAROIDROBOTCONTROLCOMMAND::ETYPE command, signed char value0, signed char value1);
	void tcpSend(int socket, WAROIDROBOTCONTROLCOMMAND::ETYPE command, unsigned short value);
	void tcpDisconnect(int socket);

	bool infoInit();
	void infoLoop();

	void onProcess(const WAROIDROBOTCONTROL& control);

private:
	int m_listenSocket;
	int m_ownerSocket;
	pthread_t m_networkThread;

private:
	static void* networkWorker(void* param);

};

#endif /* MAINMANAGER_H_ */
