/*
 * GRCBuffer.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: mirime
 */

#include "GRCBuffer.h"

#include <stdlib.h>
#include <cstring>

#include "GRCCore.h"

GRCBuffer::GRCBuffer(size_t bufferSize)
		: 	m_buffer(reinterpret_cast<char*>(malloc(bufferSize))),
			m_bufferSize(bufferSize),
			m_dataSize(0)
{
	// TODO Auto-generated constructor stub

}
GRCBuffer::~GRCBuffer()
{
	// TODO Auto-generated destructor stub
	free(m_buffer);
}

void GRCBuffer::clear()
{
	GRCMutexAutoLock autoLock(&m_mutex);
	m_dataSize = 0;
}

bool GRCBuffer::copy(const void* data, size_t dataSize)
{
	GRC_CHECK_RETFALSE(data);
	GRC_CHECK_RETFALSE(dataSize > 0);

	GRCMutexAutoLock autoLock(&m_mutex);
	GRC_CHECK_RETFALSE(dataSize <= m_bufferSize);
	memcpy(m_buffer, data, dataSize);
	m_dataSize = dataSize;

	return true;
}

bool GRCBuffer::append(const void* data, size_t dataSize)
{
	GRC_CHECK_RETFALSE(data);
	GRC_CHECK_RETFALSE(dataSize > 0);

	GRCMutexAutoLock autoLock(&m_mutex);
	GRC_CHECK_RETFALSE(m_dataSize + dataSize <= m_bufferSize);
	memcpy(m_buffer + m_dataSize, data, dataSize);
	m_dataSize += dataSize;

	return true;
}

bool GRCBuffer::append(char ch)
{
	GRCMutexAutoLock autoLock(&m_mutex);
	GRC_CHECK_RETFALSE(m_dataSize + 1 <= m_bufferSize);

	*(m_buffer + m_dataSize) = ch;
	m_dataSize += 1;

	return true;
}

bool GRCBuffer::truncateLeft(size_t dataSize)
{
	GRCMutexAutoLock autoLock(&m_mutex);
	if (dataSize > 0)
	{
		GRC_CHECK_RETFALSE(dataSize <= m_dataSize);

		if (m_dataSize == dataSize)
		{
			m_dataSize = 0;
		}
		else
		{
			m_dataSize -= dataSize;
			memmove(m_buffer, m_buffer + dataSize, m_dataSize);
		}
	}

	return true;
}

bool GRCBuffer::truncateRight(size_t dataSize)
{
	GRCMutexAutoLock autoLock(&m_mutex);
	GRC_CHECK_RETFALSE(dataSize <= m_dataSize);

	m_dataSize -= dataSize;

	return true;
}

bool GRCBuffer::postAppend(size_t dataSize)
{
	GRC_CHECK_RETFALSE(dataSize > 0);

	GRCMutexAutoLock autoLock(&m_mutex);
	GRC_CHECK_RETFALSE(m_dataSize + dataSize <= m_bufferSize);
	m_dataSize += dataSize;

	return true;
}
