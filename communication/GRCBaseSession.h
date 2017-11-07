/*
 * GRCBaseSession.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef GRCBASESESSION_H_
#define GRCBASESESSION_H_

#include <pthread.h>
#include <stddef.h>

#include "../core/GRCCore.h"
#include "../core/GRCMutex.h"
#include "../core/GRCObject.h"

class GRCBaseSession : public GRCObject
{
public:
	GRCBaseSession(size_t maxPacketSize);
	virtual ~GRCBaseSession();

public:
	virtual bool isActivate() const
	{
		return isOpen();
	}

	bool isOpen() const
	{
		return m_fd != GRC_INVALID_FD ;
	}

	void close(const char* reason);
	bool send(const void* buffer, size_t size);

protected:
	void openning();
	void recvHeartbeat(unsigned int tick);

	virtual void onOpen() = 0;
	virtual void onClose() = 0;
	virtual bool onSend(const void* data, size_t size) = 0;
	virtual void onReceiving() = 0;

protected:
	GRCMutex m_mutex;
	int m_fd;
	size_t m_maxPacketSize;
	bool m_receiving;
	unsigned int m_pingMax;
	unsigned int m_pingAvg;

private:
	pthread_t m_receiveThread;

private:
	static void* receiveWorker(void* param);
};

#endif /* GRCBASESESSION_H_ */
