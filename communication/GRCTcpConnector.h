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

#include "../core/GRCCore.h"
#include "../core/GRCCoreUtil.h"
#include "../core/GRCMutex.h"
#include "GRCCommunicator.h"
#include "GRCSockAddr.h"

template<class TCONNECTSESSION>
class GRCTcpConnectorT: public GRCCommunicatorT<TCONNECTSESSION>
{
protected:
	using GRCCommunicatorT<TCONNECTSESSION>::m_mutex;
	using GRCCommunicatorT<TCONNECTSESSION>::m_sessions;

public:
	GRCTcpConnectorT(const char* name, size_t maxSessionCount, size_t reconnectSeconds, size_t maxPacketSize)
			: 	GRCCommunicatorT<TCONNECTSESSION>(name, maxSessionCount, maxPacketSize),
				m_reconnectSeconds(reconnectSeconds),
				m_reconnectThread(GRC_INVALID_THREAD)
	{
	}
	virtual ~GRCTcpConnectorT()
	{
	}

public:
	void start()
	{
		if (m_reconnectSeconds > 0)
		{
			//reconnect check thread
			pthread_create(&m_reconnectThread, NULL, reconnectWorker, this);
			GRCCoreUtil::sleep(0.1);
		}
	}

	void stop()
	{
		GRC_INFO("[%s]stopping...", this->getObjName());

		if (m_reconnectThread != GRC_INVALID_THREAD)
		{
			pthread_cancel(m_reconnectThread);
			GRC_DEV("[%s]cancel thread", this->getObjName());
		}

		for (size_t i = 0; i < m_sessions.size(); ++i)
		{
			m_sessions[i]->m_reconnect = false;
		}

		this->closeAll();
	}

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

private:
	void reconnecting()
	{
		for (;;)
		{
			for (size_t i = 0; i < m_sessions.size(); ++i)
			{
				m_sessions[i]->reconnect();
				GRCCoreUtil::sleep(0.01);
			}
			GRCCoreUtil::sleep(m_reconnectSeconds);
		}
	}

private:
	size_t m_reconnectSeconds;
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
