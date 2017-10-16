/*
 * GRCCommunicator.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef GRCCOMMUNICATOR_H_
#define GRCCOMMUNICATOR_H_

#include <vector>

#include "../core/GRCCore.h"

template<class TSESSION>
class GRCCommunicatorT
{
public:
	typedef std::vector<TSESSION*> VecSession;

public:
	GRCCommunicatorT(const char* name, size_t maxSessionCount)
	{
		SAFE_STR_COPY(m_name, sizeof(m_name), name);
		for (size_t i = 0; i < maxSessionCount; ++i)
		{
			m_sessions.push_back(new TSESSION());
		}
	}
	virtual ~GRCCommunicatorT()
	{
		for (auto s : m_sessions)
		{
			delete s;
		}
		m_sessions.clear();
	}

public:
	TSESSION* getFirstOpenedSession()
	{
		for (size_t i = 0; i < m_sessions.size(); ++i)
		{
			if (m_sessions[i]->isOpen()) return m_sessions[i];
		}

		return NULL;
	}

protected:
	int findFreeIndex()
	{
		for (size_t i = 0; i < m_sessions.size(); ++i)
		{
			if (m_sessions[i]->isActivate() == false) return i;
		}

		return INVALID_INDEX ;
	}

	void closeAll()
	{
		for (size_t i = 0; i < m_sessions.size(); ++i)
		{
			m_sessions[i]->close("close all");
		}
	}

protected:
	char m_name[100];
	GRCMutex m_mutex;
	VecSession m_sessions;
};

#endif /* GRCCOMMUNICATOR_H_ */
