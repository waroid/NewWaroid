/*
 * GRCConnectSession.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: mirime
 */

#include "GRCConnectSession.h"

#include <sys/socket.h>
#include <unistd.h>

#include "../core/GRCCore.h"
#include "../core/GRCMutex.h"
#include "../core/GRCObject.h"

GRCConnectSession::GRCConnectSession(size_t maxPacketSize)
		: 	GRCTcpSession(maxPacketSize),
			m_reconnect(false)
{
	// TODO Auto-generated constructor stub

}

GRCConnectSession::~GRCConnectSession()
{
	// TODO Auto-generated destructor stub
}

bool GRCConnectSession::connect(const GRCSockAddr& targetSockAddr, bool reconnect)
{
	GRC_CHECK_RETFALSE(targetSockAddr.isValid());

	{
		GRCMutexAutoLock autoLock(&m_mutex);
		m_reconnect = reconnect;
		m_remoteSockAddr = targetSockAddr;
	}

	if (reconnect)
	{
		GRC_INFO("[%s]connecting... target=%s", getObjName(), *m_remoteSockAddr);
	}
	else
	{
		GRC_CHECK_RETFALSE(connecting());

		this->openning();

		GRC_INFO("[%s]connected.", getObjName());
	}

	return true;
}

void GRCConnectSession::reconnect()
{
	if (m_reconnect && isOpen() == false && m_remoteSockAddr.isValid())
	{
		GRC_CHECK_RETURN(connecting());

		this->openning();

		GRC_INFO("[%s]reconnected.", getObjName());
	}
}

void GRCConnectSession::onClose()
{
	if (m_reconnect == false)
	{
		m_remoteSockAddr.clear();
	}

	GRCTcpSession::onClose();
}


bool GRCConnectSession::connecting()
{
	GRC_CHECK_RETFALSE(m_remoteSockAddr.isValid());
	GRC_CHECK_RETFALSE(m_fd == GRC_INVALID_FD);

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	GRC_CHECK_RETFALSE(fd != GRC_INVALID_FD);
	GRC_CHECK_FUNC_RETFALSE(::connect(fd, m_remoteSockAddr, GRCSockAddr::LEN) == 0, ::close(fd));

	GRCMutexAutoLock autoLock(&m_mutex);
	sockaddr sockAddr;
	bzero(&sockAddr, sizeof(sockAddr));
	socklen_t len = sizeof(sockAddr);
	::getsockname(fd, &sockAddr, &len);
	m_localSockAddr.set(&sockAddr);
	m_fd = fd;

	return true;
}

