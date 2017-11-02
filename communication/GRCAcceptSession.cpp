/*
 * GRCAcceptSession.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: mirime
 */

#include "GRCAcceptSession.h"

#include <unistd.h>

#include "../core/GRCCore.h"
#include "../core/GRCMutex.h"
#include "../core/GRCObject.h"

GRCAcceptSession::GRCAcceptSession(size_t maxPacketSize)
		: GRCTcpSession(maxPacketSize)
{
	// TODO Auto-generated constructor stub

}

GRCAcceptSession::~GRCAcceptSession()
{
	// TODO Auto-generated destructor stub
}

void GRCAcceptSession::accepted(int fd, const GRCSockAddr& localSockAddr, const GRCSockAddr& remoteSockAddr)
{
	GRC_CHECK_RETURN(fd != GRC_INVALID_FD);
	GRC_CHECK_RETURN(localSockAddr.isValid());
	GRC_CHECK_RETURN(remoteSockAddr.isValid());

	{
		GRCMutexAutoLock autoLock(&m_mutex);
		GRC_CHECK_FUNC_RETURN(m_fd == GRC_INVALID_FD, ::close(fd));
		m_fd = fd;
		m_localSockAddr = localSockAddr;
		m_remoteSockAddr = remoteSockAddr;
	}

	this->openning();

	GRC_INFO("[%s]accepted.", getObjName());
}
