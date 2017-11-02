/*
 * GRCTcpConnector.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef GRCTCPCONNECTOR_H_
#define GRCTCPCONNECTOR_H_

#include <pthread.h>
#include <stddef.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

#include "../core/GRCCore.h"
#include "../core/GRCLogger.h"
#include "../core/GRCMutex.h"
#include "GRCCommunicator.h"
#include "GRCSockAddr.h"

template<class TTCPSESSION>
class GRCTcpConnectorT: public GRCCommunicatorT<TTCPSESSION>
{
protected:
	using GRCCommunicatorT<TTCPSESSION>::m_mutex;
	using GRCCommunicatorT<TTCPSESSION>::m_sessions;

public:
	GRCTcpConnectorT(const char* name, size_t maxSessionCount, size_t reconnectSeconds, size_t maxPacketSize)
			: 	GRCCommunicatorT<TTCPSESSION>(name, maxSessionCount, maxPacketSize),
				m_reconnectSeconds(reconnectSeconds),
				m_reconnectThread(GRC_INVALID_THREAD)
	{
		if (m_reconnectSeconds > 0)
		{
			//reconnect check thread
			GRC_CHECK_RETURN(pthread_create(&m_reconnectThread, NULL, reconnectWorker, this) == 0);
		}
	}
	virtual ~GRCTcpConnectorT()
	{
	}

public:
	bool connect(const char* ip, int port, bool reconnect)
	{
		GRC_CHECK_RETFALSE(ip);
		GRC_CHECK_RETFALSE(port > 0);

		GRCSockAddr sockAddr(ip, port);
		GRC_CHECK_RETFALSE(sockAddr.isValid());

		GRCMutexAutoLock autoLock(&m_mutex);

		size_t index = this->findFreeIndex();
		GRC_CHECK_RETFALSE(index != GRC_INVALID_INDEX);
		GRC_CHECK_RETFALSE(m_sessions[index]->connect(sockAddr, reconnect));

		return true;
	}

	void stop()
	{
		GRC_INFO("[%s]stopping...", this->getObjName());

		if (m_reconnectThread != GRC_INVALID_THREAD)
		{
			if (pthread_cancel(m_reconnectThread) == 0)
			{
				pthread_join(m_reconnectThread, NULL);
			}
			GRC_DEV("[%s]cancel thread", this->getObjName());
		}

		this->closeAll();
	}

private:
	void reconnecting()
	{
		for (;;)
		{
			for (size_t i = 0; i < m_sessions.size(); ++i)
			{
				m_sessions[i]->reconnect();
				usleep(10000);
			}
			sleep(m_reconnectSeconds);
		}
	}

protected:
	size_t m_reconnectSeconds;

private:
	pthread_t m_reconnectThread;

private:
	static void* reconnectWorker(void* param)
	{
		GRCTcpConnectorT* tcpConnector = (GRCTcpConnectorT*)param;

		GRC_INFO("[%s]start reconnect thread(0x%x)", tcpConnector->getObjName(), pthread_self());
		tcpConnector->reconnecting();
		GRC_INFO("[%s]stop reconnect thread(0x%x)", tcpConnector->getObjName(), pthread_self());

		return NULL;
	}
};

#endif /* GRCTCPCONNECTOR_H_ */
