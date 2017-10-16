/*
 * GRCBaseSession.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef GRCBASESESSION_H_
#define GRCBASESESSION_H_

#include <pthread.h>

#include "../core/GRCCore.h"
#include "../core/GRCMutex.h"

class GRCBaseSession
{
public:
	GRCBaseSession();
	virtual ~GRCBaseSession();

public:
	bool isOpen() const
	{
		return m_fd != INVALID_FD ;
	}

	bool isActivate() const
	{
		return m_fd != INVALID_FD || m_reconnect;
	}

	void close(const char* reason);

protected:
	void openning();

	virtual void onOpen()
	{
	}
	virtual void onClose()
	{
	}

	virtual void onReceiving() = 0;

protected:
	char m_name[100];
	GRCMutex m_mutex;
	int m_fd;
	bool m_reconnect;
	pthread_t m_thread;

private:
	static void* recvWorker(void* param);
};

#endif /* GRCBASESESSION_H_ */
