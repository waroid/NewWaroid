/*
 * GRCMutex.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef GRCMUTEX_H_
#define GRCMUTEX_H_

#include <pthread.h>
#include <functional>

class GRCMutex
{
public:
	GRCMutex()
			: 	m_mutex(PTHREAD_MUTEX_INITIALIZER),
				m_cond(PTHREAD_COND_INITIALIZER)
	{
	}
	virtual ~GRCMutex()
	{
	}

public:
	void lock()
	{
		pthread_mutex_lock(&m_mutex);
	}

	void unlock()
	{
		pthread_mutex_unlock(&m_mutex);
	}

	void wait()
	{
		pthread_mutex_lock(&m_mutex);
		pthread_cond_wait(&m_cond, &m_mutex);
		pthread_mutex_unlock(&m_mutex);
	}

	void signal()
	{
		pthread_mutex_lock(&m_mutex);
		pthread_cond_signal(&m_cond);
		pthread_mutex_unlock(&m_mutex);
	}

	void signalIf(std::function<bool()> func)
	{
		pthread_mutex_lock(&m_mutex);
		if (func())
		{
			pthread_cond_signal(&m_cond);
		}
		pthread_mutex_unlock(&m_mutex);
	}

private:
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
};

class GRCMutexAutoLock
{
public:
	GRCMutexAutoLock(GRCMutex* mutex)
			: m_mutex(mutex)
	{
		if (m_mutex) m_mutex->lock();
	}
	~GRCMutexAutoLock()
	{
		if (m_mutex) m_mutex->unlock();
	}

private:
	GRCMutex* m_mutex;
};

#endif /* GRCMUTEX_H_ */
