/*
 * GRCCounter.h
 *
 *  Created on: Nov 17, 2017
 *      Author: mirime
 */

#ifndef CORE_GRCCOUNTER_H_
#define CORE_GRCCOUNTER_H_

#include "GRCMutex.h"

class GRCCounter
{
private:
	GRCCounter(const GRCCounter&);
	GRCCounter& operator=(const GRCCounter&);

public:
	GRCCounter()
			: m_value(0)
	{
	}

public:
	bool isZero() const
	{
		return m_value == 0;
	}

	void exchange(unsigned int value)
	{
		GRCMutexAutoLock autoLock(&m_lock);
		m_value = value;
	}

	unsigned int inc()
	{
		GRCMutexAutoLock autoLock(&m_lock);
		m_value++;
		return m_value;
	}

	unsigned int dec()
	{
		GRCMutexAutoLock autoLock(&m_lock);
		m_value--;
		return m_value;
	}

	bool operator==(const GRCCounter& counter) const
	{
		return m_value == counter.m_value;
	}
	bool operator!=(const GRCCounter& counter) const
	{
		return m_value != counter.m_value;
	}

	unsigned int operator*() const
	{
		return m_value;
	}

private:
	GRCMutex m_lock;
	unsigned int m_value;
};

#endif /* CORE_GRCCOUNTER_H_ */
