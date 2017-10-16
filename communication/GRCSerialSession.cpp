/*
 * GRCSerialSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "GRCSerialSession.h"

#include <pthread.h>
#include <unistd.h>
#include <wiringSerial.h>
#include <cerrno>
#include <cstring>

#include "../core/GRCCore.h"
#include "../core/GRCLogger.h"
#include "../core/GRCMutex.h"

GRCSerialSession::GRCSerialSession()
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

	GRCMutexAutoLock autoLock(&m_mutex);
	GRC_CHECK_RETFALSE(m_fd == INVALID_FD);

	m_fd = serialOpen(device, baud);
	GRC_CHECK_RETFALSE(m_fd != INVALID_FD);

	SAFE_STR_COPY(m_name, sizeof(m_name), device);

	this->openning();

	return true;
}

void GRCSerialSession::send(const unsigned char* data, int size)
{
	GRC_CHECK_RETURN(data);
	GRC_CHECK_RETURN(size>0);

	for (int i = 0; i < size; ++ i)
	{
		serialPutchar(m_fd, data[i]);
	}
	serialFlush(m_fd);
}

void GRCSerialSession::onReceiving()
{
	char buffer[1024];
	int offset = 0;

	for (;;)
	{
		int len = recv(buffer + offset, sizeof(buffer) - offset);

		const char* data = buffer;
		int dataSize = len + offset;

		for (;;)
		{
			int skipSize = 0;
			int packetSize = onParsing(data, dataSize, skipSize);
			if (packetSize > 0)
			{
				data += skipSize;
				onPacket(data, packetSize);
				data += packetSize;
				dataSize -= (skipSize + packetSize);
			}
			else if (packetSize == 0)
			{
				dataSize -= skipSize;

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
				data += skipSize;
				dataSize -= skipSize;
			}
		}
	}
}

int GRCSerialSession::recv(char* buffer, int len)
{
	int received = 0;
	while (received == 0)
	{
		::usleep(100000);

		while (serialDataAvail(m_fd))
		{
			buffer[received++] = serialGetchar(m_fd);
			if (received >= len) return received;
		}
	}

	return received;
}
