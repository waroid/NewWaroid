/*
 * GRCTcpListener.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef GRCTCPLISTENER_H_
#define GRCTCPLISTENER_H_

#include "../core/GRCCore.h"
#include "GRCSockAddr.h"
#include "GRCCommunicator.h"

template<class TTCPSESSION>
class GRCTcpListenerT: public GRCCommunicatorT<TTCPSESSION>
{
protected:
	using GRCCommunicatorT<TTCPSESSION>::m_name;
	using GRCCommunicatorT<TTCPSESSION>::m_mutex;
	using GRCCommunicatorT<TTCPSESSION>::m_sessions;

public:
	GRCTcpListenerT(const char* name, size_t maxSessionCount)
			: GRCCommunicatorT<TTCPSESSION>(name, maxSessionCount), m_fd(INVALID_FD), m_thread(INVALID_THREAD)
	{
	}
	virtual ~GRCTcpListenerT()
	{
	}

public:
	bool listen(int port)
	{
		GRC_CHECK_RETFALSE(port > 0);

		m_sockAddr.set(NULL, port);
		GRC_CHECK_RETFALSE(m_sockAddr.isValid());

		auto efunc = [this]()
		{
			close(m_fd);
			m_fd = INVALID_FD;
		};

		GRCMutexAutoLock autoLock(&m_mutex);

		GRC_CHECK_RETFALSE(m_fd==INVALID_FD);

		m_fd = socket(AF_INET, SOCK_STREAM, 0);
		GRC_CHECK_RETFALSE(m_fd != INVALID_FD);

		{
			int optval = 1;
			GRC_CHECK_FUNC_RETFALSE(setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))==0, efunc());
		}

		GRC_CHECK_FUNC_RETFALSE(::bind(m_fd, m_sockAddr, GRCSockAddr::LEN) == 0, efunc());
		GRC_CHECK_FUNC_RETFALSE(::listen(m_fd, 10) == 0, efunc());

		pthread_create(&m_thread, NULL, acceptWorker, this);

		GRC_LOG("[%s]listened. address=%s", m_name, *m_sockAddr);

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

		if (m_fd != INVALID_FD)
		{
			close(m_fd);
			m_fd = INVALID_FD;
			GRC_DEV("[%s]close listen socket", m_name);
		}

		this->closeAll();
	}

private:
	void accepting()
	{
		sockaddr sockAddr;
		socklen_t len = sizeof(sockAddr);

		for (;;)
		{
			int fd = accept(m_fd, &sockAddr, &len);

			GRCMutexAutoLock autoLock(&m_mutex);
			int index = this->findFreeIndex();
			if (index == INVALID_INDEX)
			{
				close(fd);
				GRC_LOG("[%s]close accept socket. reason=not exist free");
			}
			else
			{
				m_sessions[index]->accepted(fd, m_sockAddr, &sockAddr);
			}
		}
	}

protected:
	int m_fd;
	GRCSockAddr m_sockAddr;
	pthread_t m_thread;

private:
	static void* acceptWorker(void* param)
	{
		GRCTcpListenerT* tcpListener = (GRCTcpListenerT*) param;

		GRC_LOG("[%s]start accept thread(%u)", tcpListener->m_name, pthread_self());
		tcpListener->accepting();
		GRC_LOG("[%s]stop accept thread(%u)", tcpListener->m_name, pthread_self());

		return NULL;
	}
}
;

#endif /* GRCTCPLISTENER_H_ */
