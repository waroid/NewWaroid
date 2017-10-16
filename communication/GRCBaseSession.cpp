/*
 * GRCBaseSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "GRCBaseSession.h"

#include <unistd.h>
#include <cstring>

#include "../core/GRCLogger.h"

GRCBaseSession::GRCBaseSession()
		: m_fd(INVALID_FD), m_reconnect(false), m_thread(INVALID_THREAD)
{
	// TODO Auto-generated constructor stub
	bzero(m_name, sizeof(m_name));
}

GRCBaseSession::~GRCBaseSession()
{
	// TODO Auto-generated destructor stub
}

void GRCBaseSession::close(const char* reason)
{
	GRCMutexAutoLock autoLock(&m_mutex);
	if (m_fd != INVALID_FD)
	{
		onClose();
		::close(m_fd);
		GRC_LOG("[%s]closed. reason=%s", m_name, reason);
		m_fd = INVALID_FD;
	}
}

void GRCBaseSession::openning()
{
	onOpen();
	pthread_create(&m_thread, NULL, recvWorker, this);
}

void* GRCBaseSession::recvWorker(void* param)
{
	GRCBaseSession* session = (GRCBaseSession*) param;

	GRC_LOG("[%s]start thread(%d)", session->m_name, pthread_self());
	session->onReceiving();
	GRC_LOG("[%s]stop thread(%d)", session->m_name, pthread_self());

	return NULL;
}
