/*
 * GRCBaseSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "GRCBaseSession.h"

#include <unistd.h>
#include <cstring>

#include "../core/GRCCoreUtil.h"

GRCBaseSession::GRCBaseSession(size_t maxPacketSize)
		: 	m_fd(GRC_INVALID_FD),
			m_maxPacketSize(maxPacketSize),
			m_pingMax(0),
			m_pingAvg(0),
			m_receiveThread(GRC_INVALID_THREAD)

{
	// TODO Auto-generated constructor stub
}

GRCBaseSession::~GRCBaseSession()
{
	// TODO Auto-generated destructor stub
}

void GRCBaseSession::close(const char* reason)
{
	m_mutex.lock();
	int fd = m_fd;
	m_fd = GRC_INVALID_FD;
	m_mutex.unlock();

	if (fd != GRC_INVALID_FD)
	{
		if (m_receiveThread != GRC_INVALID_THREAD)
		{
			if (pthread_cancel(m_receiveThread) == 0)
			{
				pthread_join(m_receiveThread, NULL);
			}
			GRC_DEV("[%s]cancel thread", getObjName());
		}
		onClose();
		::close(fd);
		GRC_LOG("[%s]closed. reason=%s", getObjName(), reason);
	}
}

bool GRCBaseSession::send(const void* data, size_t size)
{
	GRC_CHECK_RETFALSE(data);
	GRC_CHECK_RETFALSE(size > 0);
	GRC_CHECK_RETFALSE(size <= m_maxPacketSize);

	return onSend(data, size);
}

void GRCBaseSession::openning()
{
	onOpen();
	pthread_create(&m_receiveThread, NULL, receiveWorker, this);
}

void GRCBaseSession::recvHeartbeat(unsigned int tick)
{
	unsigned int ping = GRCCoreUtil::getTickCount() - tick;
	if (m_pingMax < ping) m_pingMax = ping;
	m_pingAvg = static_cast<unsigned int>(m_pingAvg * 0.75 + ping * 0.25);
}

void* GRCBaseSession::receiveWorker(void* param)
{
	GRCBaseSession* session = (GRCBaseSession*)param;

	GRC_LOG("[%s]start receive thread(0x%x)", session->getObjName(), pthread_self());
	session->onReceiving();
	GRC_LOG("[%s]stop receive thread(0x%x)", session->getObjName(), pthread_self());

	return NULL;
}

