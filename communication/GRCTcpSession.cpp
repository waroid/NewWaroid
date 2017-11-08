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
		: GRCBaseSession(maxPacketSize)
{
	// TODO Auto-generated constructor stub
}

GRCTcpSession::~GRCTcpSession()
{
	// TODO Auto-generated destructor stub
}

void GRCTcpSession::onOpen()
{
	updateObjName("%s-%s", *m_localSockAddr, *m_remoteSockAddr);

	int optval = 1;
	setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
}

void GRCTcpSession::onClose()
{
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
		if (len == 0)
			close("remote");
		else
			close("failed recv");
	};

	GRCBuffer buffer(m_maxPacketSize * 100);
	int offset = 0;

	while (m_receiving)
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
				buffer.truncateLeft(offset);
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

