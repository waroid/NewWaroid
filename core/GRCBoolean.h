/*
 * GRCBoolean.h
 *
 *  Created on: Nov 17, 2017
 *      Author: mirime
 */

#ifndef CORE_GRCBOOLEAN_H_
#define CORE_GRCBOOLEAN_H_

#include "GRCMutex.h"

class GRCBoolean
{
public:
	GRCBoolean(bool b = false)
			: m_b(b)
	{
	}

public:
	// return: exchanged
	bool update(bool b)
	{
		GRCMutexAutoLock autoLock(&m_lock);
		if (m_b == b) return false;

		m_b = b;
		return true;
	}

	GRCBoolean& operator=(bool b)
	{
		GRCMutexAutoLock autoLock(&m_lock);
		m_b = b;
		return *this;
	}

	bool operator==(bool b) const
	{
		return m_b == b;
	}
	bool operator!=(bool b) const
	{
		return m_b != b;
	}

	operator bool() const
	{
		return m_b;
	}
	bool operator*() const
	{
		return m_b;
	}

private:
	GRCMutex m_lock;
	bool m_b;
};

#endif /* CORE_GRCBOOLEAN_H_ */
