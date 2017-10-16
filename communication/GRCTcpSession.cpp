/*
 * GRCTcpSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "GRCTcpSession.h"

#include <unistd.h>
#include <stdio.h>
#include <netinet/tcp.h>

#include "../core/GRCCore.h"

GRCTcpSession::GRCTcpSession()
{
	// TODO Auto-generated constructor stub
}

GRCTcpSession::~GRCTcpSession()
{
	// TODO Auto-generated destructor stub
}

void GRCTcpSession::accepted(int fd, const GRCSockAddr& localSockAddr, const GRCSockAddr& remoteSockAddr)
{
	GRC_CHECK_RETURN(fd!=INVALID_FD);
	GRC_CHECK_RETURN(localSockAddr.isValid());
	GRC_CHECK_RETURN(remoteSockAddr.isValid());

	GRCMutexAutoLock autoLock(&m_mutex);
	GRC_CHECK_FUNC_RETURN(m_fd == INVALID_FD, ::close(fd));

	m_fd = fd;

	this->openning();

	GRC_LOG("[%s]accepted.", m_name);
}

bool GRCTcpSession::connect(const GRCSockAddr& targetSockAddr, bool reconnect)
{
	GRC_CHECK_RETFALSE(targetSockAddr.isValid());

	GRCMutexAutoLock autoLock(&m_mutex);
	m_reconnect = reconnect;
	m_remoteSockAddr = targetSockAddr;

	GRC_CHECK_RETFALSE(connecting());

	this->openning();

	GRC_LOG("[%s]connected.", m_name);

	return true;
}

void GRCTcpSession::reconnect()
{
	GRCMutexAutoLock autoLock(&m_mutex);
	if (m_reconnect)
	{
		GRC_CHECK_RETURN(connecting());

		this->openning();

		GRC_LOG("[%s]reconnected.", m_name);
	}
}

void GRCTcpSession::onOpen()
{
	GRCBaseSession::onOpen();

	sprintf(m_name, "%s-%s", *m_localSockAddr, *m_remoteSockAddr);

	int optval = 1;
	setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
}

void GRCTcpSession::onClose()
{
	m_remoteSockAddr.clear();

	GRCBaseSession::onClose();
}

void GRCTcpSession::onReceiving()
{
	char buffer[1024];
	int offset = 0;

	auto efunc = [this](int len)
	{
		if (len == 0) close("remote");
		else close("failed recv");
	};

	for (;;)
	{
		int len = ::recv(m_fd, buffer + offset, sizeof(buffer) - offset, 0);
		GRC_CHECK_FUNC_RETURN(len > 0, efunc(len));

		const char* data = buffer;
		int dataSize = len + offset;

		for (;;)
		{
			int packetSize = onParsing(data, dataSize);
			if (packetSize > 0)
			{
				onPacket(data, packetSize);
				data += packetSize;
				dataSize -= packetSize;
			}
			else if (packetSize == 0)
			{
				if (dataSize > 0)
				{
					memmove(buffer, data + (len + offset - dataSize), dataSize);
					offset = dataSize;
				}
				else
				{
					offset = 0;
				}
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

	auto efunc = [this]()
	{
		::close(m_fd);
		m_fd = INVALID_FD;
	};

	GRC_CHECK_RETFALSE(m_fd == INVALID_FD);

	m_fd = socket(AF_INET, SOCK_STREAM, 0);
	GRC_CHECK_RETFALSE(m_fd == INVALID_FD);

	GRC_CHECK_FUNC_RETFALSE(::connect(m_fd, m_remoteSockAddr, GRCSockAddr::LEN) == 0, efunc());

	sockaddr sockAddr;
	socklen_t len = sizeof(sockAddr);
	::getsockname(m_fd, &sockAddr, &len);
	m_localSockAddr.set(&sockAddr);

	return true;
}
