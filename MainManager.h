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
	void tcpSend(int socket, WAROIDROBOTCOMMAND::ETYPE command, int data0, int data1);
	void tcpSend(int socket, WAROIDROBOTCOMMAND::ETYPE command, int data);
	void tcpDisconnect(int socket);

	void onProcess(const WAROIDROBOTDATA& data);

private:
	int m_listenSocket;
	int m_ownerSocket;
	char m_ownerAddress[30];
	pthread_t m_networkThread;
	bool m_opendCamera;

private:
	static void* networkWorker(void* param);

};

#endif /* MAINMANAGER_H_ */
