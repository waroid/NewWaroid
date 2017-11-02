/*
 * GRCBuffer.h
 *
 *  Created on: Nov 1, 2017
 *      Author: mirime
 */

#ifndef SRC_CORE_GRCBUFFER_H_
#define SRC_CORE_GRCBUFFER_H_

#include <stddef.h>

#include "GRCMutex.h"

class GRCBuffer
{
public:
	GRCBuffer(size_t bufferSize);
	virtual ~GRCBuffer();

public:
	bool isEmpty() const
	{
		return m_dataSize == 0;
	}
	bool isFull() const
	{
		return m_bufferSize == m_dataSize;
	}

	const char* getData() const
	{
		return reinterpret_cast<const char*>(m_buffer);
	}

	size_t getDataSize() const
	{
		return m_dataSize;
	}

	size_t getBufferSize() const
	{
		return m_bufferSize;
	}

	void* getFreeBuffer()
	{
		return (reinterpret_cast<char*>(m_buffer) + m_dataSize);
	}

	size_t getFreeBufferSize() const
	{
		return m_bufferSize - m_dataSize;
	}

	void clear();
	bool copy(const void* data, size_t dataSize);
	bool append(const void* data, size_t dataSize);
	bool append(char ch);
	bool truncate(size_t dataSize);
	bool postAppend(size_t dataSize);

private:
	GRCMutex m_mutex;
	void* m_buffer;
	size_t m_bufferSize;
	size_t m_dataSize;
};

#endif /* SRC_CORE_GRCBUFFER_H_ */
