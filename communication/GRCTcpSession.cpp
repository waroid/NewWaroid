/*
 * GRCTcpSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "GRCTcpSession.h"

#include <netinet/tcp.h>
#include <stddef.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

#include "../core/GRCBuffer.h"
#include "../core/GRCCore.h"
#include "../core/GRCMutex.h"
#include "../core/GRCObject.h"

GRCTcpSession::GRCTcpSession(size_t maxPacketSize)
		: 	GRCBaseSession(maxPacketSize),
			m_reconnect(false)
{
	// TODO Auto-generated constructor stub
}

GRCTcpSession::~GRCTcpSession()
{
	// TODO Auto-generated destructor stub
}

void GRCTcpSession::accepted(int fd, const GRCSockAddr& localSockAddr, const GRCSockAddr& remoteSockAddr)
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

	GRC_LOG("[%s]accepted.", getObjName());
}

bool GRCTcpSession::connect(const GRCSockAddr& targetSockAddr, bool reconnect)
{
	GRC_CHECK_RETFALSE(targetSockAddr.isValid());

	{
		GRCMutexAutoLock autoLock(&m_mutex);
		m_reconnect = reconnect;
		m_remoteSockAddr = targetSockAddr;
	}

	GRC_CHECK_RETFALSE(connecting());

	this->openning();

	GRC_LOG("[%s]connected.", getObjName());

	return true;
}

void GRCTcpSession::reconnect()
{
	if (m_reconnect)
	{
		GRC_CHECK_RETURN(connecting());

		this->openning();

		GRC_LOG("[%s]reconnected.", getObjName());
	}
}

void GRCTcpSession::onOpen()
{
	updateObjName("%s-%s", *m_localSockAddr, *m_remoteSockAddr);

	int optval = 1;
	setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
}

void GRCTcpSession::onClose()
{
	m_remoteSockAddr.clear();
}

bool GRCTcpSession::onSend(const void* data, size_t size)
{
	int len = ::send(m_fd, data, size, 0);
	GRC_CHECKV_RETFALSE(len > 0, "invalid sent. size=%d:%d", len, size);

	return true;
}

void GRCTcpSession::onReceiving()
{
	auto eclose = [this](int len)
	{
		if (len == 0) close("remote");
		else close("failed recv");
	};

	GRCBuffer buffer(m_maxPacketSize * 100);
	int offset = 0;

	for (;;)
	{
		int len = ::recv(m_fd, buffer.getFreeBuffer(), buffer.getFreeBufferSize(), 0);
		GRC_CHECK_FUNC_RETURN(len > 0, eclose(len));
		GRC_CHECK_FUNC_RETURN(buffer.postAppend(len), eclose(-1));
		offset = 0;

		for (;;)
		{
			int packetSize = onParsing(buffer.getData() + offset, buffer.getDataSize() - offset);
			if (packetSize > 0)
			{
				onPacket(buffer.getData() + offset, packetSize);
				offset += packetSize;
			}
			else if (packetSize == 0)
			{
				buffer.truncate(offset);
				break;
			}
			else
			{
				close("failed parsing");
				return;
			}
		}
	}
}

bool GRCTcpSession::connecting()
{
	GRC_CHECK_RETFALSE(m_remoteSockAddr.isValid());
	GRC_CHECK_RETFALSE(m_fd == GRC_INVALID_FD);

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	GRC_CHECK_RETFALSE(fd != GRC_INVALID_FD);
	GRC_CHECK_FUNC_RETFALSE(::connect(fd, m_remoteSockAddr, GRCSockAddr::LEN) == 0, ::close(fd));

	GRCMutexAutoLock autoLock(&m_mutex);
	sockaddr sockAddr;
	socklen_t len = sizeof(sockAddr);
	::getsockname(m_fd, &sockAddr, &len);
	m_localSockAddr.set(&sockAddr);
	m_fd = fd;

	return true;
}
