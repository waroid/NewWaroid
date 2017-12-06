/*
 * GRCTcpListener.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef GRCTCPLISTENER_H_
#define GRCTCPLISTENER_H_

#include <pthread.h>
#include <stddef.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../core/GRCCore.h"
#include "../core/GRCMutex.h"
#include "GRCCommunicator.h"
#include "GRCSockAddr.h"

template<class TACCEPTSESSION>
class GRCTcpListenerT: public GRCCommunicatorT<TACCEPTSESSION>
{
protected:
	using GRCCommunicatorT<TACCEPTSESSION>::m_mutex;
	using GRCCommunicatorT<TACCEPTSESSION>::m_sessions;

public:
	GRCTcpListenerT(const char* name, size_t maxSessionCount, size_t maxPacketSize)
			: 	GRCCommunicatorT<TACCEPTSESSION>(name, maxSessionCount, maxPacketSize),
				m_fd(GRC_INVALID_FD),
				m_acceptThread(GRC_INVALID_THREAD)
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

		auto eclose = [this]()
		{
			::close(m_fd);
			m_fd = GRC_INVALID_FD;
		};

		GRCMutexAutoLock autoLock(&m_mutex);

		GRC_CHECK_RETFALSE(m_fd == GRC_INVALID_FD);

		m_fd = socket(AF_INET, SOCK_STREAM, 0);
		GRC_CHECK_RETFALSE(m_fd != GRC_INVALID_FD);

		{
			int optval = 1;
			GRC_CHECK_FUNC_RETFALSE(setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))==0, eclose());
		}

		GRC_CHECK_FUNC_RETFALSE(::bind(m_fd, m_sockAddr, GRCSockAddr::LEN) == 0, eclose());
		GRC_CHECK_FUNC_RETFALSE(::listen(m_fd, 10) == 0, eclose());

		pthread_create(&m_acceptThread, NULL, acceptWorker, this);
		GRCCoreUtil::sleep(0.1);

		GRC_INFO("[%s]listened. address=%s", this->getObjName(), *m_sockAddr);

		return true;
	}

	void stop()
	{
		GRC_INFO("[%s]stopping...", this->getObjName());

		if (m_acceptThread != GRC_INVALID_THREAD)
		{
			pthread_cancel(m_acceptThread);
			GRC_INFO("[%s]cancel thread", this->getObjName());
		}

		if (m_fd != GRC_INVALID_FD)
		{
			::close(m_fd);
			m_fd = GRC_INVALID_FD;
			GRC_INFO("[%s]close listen socket", this->getObjName());
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
			size_t index = this->findFreeIndex();
			if (index == GRC_INVALID_INDEX)
			{
				::close(fd);
				GRC_ERR("[%s]close accept socket. reason=not exist free");
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

private:
	pthread_t m_acceptThread;

private:
	static void* acceptWorker(void* param)
	{
		GRCTcpListenerT* tcpListener = (GRCTcpListenerT*)param;

		GRC_INFO("[%s]start accept thread(0x%x)", tcpListener->getObjName(), pthread_self());
		tcpListener->accepting();
		GRC_INFO("[%s]stop accept thread(0x%x)", tcpListener->getObjName(), pthread_self());

		return NULL;
	}
};

#endif /* GRCTCPLISTENER_H_ */
