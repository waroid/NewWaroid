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
	using GRCCommunicatorT<TTCPSESSION>::m_name;
	using GRCCommunicatorT<TTCPSESSION>::m_mutex;
	using GRCCommunicatorT<TTCPSESSION>::m_sessions;

public:
	GRCTcpConnectorT(const char* name, size_t maxSessionCount, int reconnectSeconds)
			: GRCCommunicatorT<TTCPSESSION>(name, maxSessionCount), m_thread(INVALID_THREAD), m_reconnectSeconds(reconnectSeconds)
	{
		if (m_reconnectSeconds > 0)
		{
			//reconnect check thread
			GRC_CHECK_RETURN(pthread_create(&m_thread, NULL, reconnectWorker, this) == 0);
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

		int index = this->findFreeIndex();
		GRC_CHECK_RETFALSE(index != INVALID_INDEX);
		GRC_CHECK_RETFALSE(m_sessions[index]->connect(sockAddr, reconnect));

		return true;
	}

	void stop()
	{
		GRC_LOG("[%s]stopping...", m_name);

		if (m_thread != INVALID_THREAD)
		{
			if (pthread_cancel(m_thread) == 0)
			{
				pthread_join(m_thread, NULL);
			}
			GRC_DEV("[%s]cancel thread", m_name);
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
	pthread_t m_thread;
	int m_reconnectSeconds;

private:
	static void* reconnectWorker(void* param)
	{
		GRCTcpConnectorT* tcpConnector = (GRCTcpConnectorT*) param;

		GRC_LOG("[%s]start reconnect thread(%u)", tcpConnector->m_name, pthread_self());
		tcpConnector->reconnecting();
		GRC_LOG("[%s]stop reconnect thread(%u)", tcpConnector->m_name, pthread_self());

		return NULL;
	}
};

#endif /* GRCTCPCONNECTOR_H_ */
