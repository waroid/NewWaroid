/*
 * GRCSerialSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "GRCSerialSession.h"

#include <stddef.h>
#include <unistd.h>
#ifdef __RPI__
#include <wiringSerial.h>
#endif
#include <cstring>

#include "../core/GRCBuffer.h"
#include "../core/GRCCore.h"
#include "../core/GRCMutex.h"

GRCSerialSession::GRCSerialSession(size_t maxPacketSize)
		: GRCBaseSession(maxPacketSize)
{
	// TODO Auto-generated constructor stub
}

GRCSerialSession::~GRCSerialSession()
{
	// TODO Auto-generated destructor stub
}

bool GRCSerialSession::open(const char* device, int baud)
{
	GRC_CHECK_RETFALSE(device);
	GRC_CHECK_RETFALSE(baud > 0);

	{
		GRCMutexAutoLock autoLock(&m_mutex);
		GRC_CHECK_RETFALSE(m_fd == GRC_INVALID_FD);
#ifdef __RPI__
		m_fd = serialOpen(device, baud);
#else
		m_fd = 999999;
#endif
		GRC_CHECK_RETFALSE(m_fd != GRC_INVALID_FD);
	}

	updateObjName(device);

	this->openning();

	return true;
}

void GRCSerialSession::onOpen()
{
}

void GRCSerialSession::onClose()
{
}

bool GRCSerialSession::onSend(const void* data, size_t size)
{
	GRCMutexAutoLock autoLock(&m_mutex);
#ifdef __RPI__
	const char* d = reinterpret_cast<const char*>(data);
	for (size_t i = 0; i < size; ++i)
	{
		serialPutchar(m_fd, d[i]);
	}
	serialFlush(m_fd);
#endif

	return true;
}

void GRCSerialSession::onReceiving()
{
	GRCBuffer buffer(m_maxPacketSize * 100);

	int offset = 0;
	int skipSize = 0;

	while (m_receiving)
	{
		recv(buffer);

		offset = 0;

		for (;;)
		{
			skipSize = 0;
			int packetSize = onParsing(buffer.getData() + offset, buffer.getDataSize() - offset, skipSize);
			if (packetSize > 0)
			{
				offset += skipSize;
				onPacket(buffer.getData() + offset, packetSize);
				offset += packetSize;
			}
			else if (packetSize == 0)
			{
				offset += skipSize;
				buffer.truncate(offset);
				break;
			}
			else
			{
				offset += skipSize;
			}
		}
	}
}

void GRCSerialSession::recv(GRCBuffer& buffer)
{
	while (m_receiving)
	{
		::usleep(100000);

		{
			GRCMutexAutoLock autolock(&m_mutex);
#ifdef __RPI__
			int received = 0;
			while (serialDataAvail(m_fd))
			{
				if (buffer.isFull()) return;

				buffer.append(serialGetchar(m_fd));
				received++;
			}
			if (received > 0)
			{
				GRC_DEV("received. size=%d", received);
				return;
			}
#endif
		}
	}
}
