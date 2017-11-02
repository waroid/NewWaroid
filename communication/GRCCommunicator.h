/*
 * GRCCommunicator.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef GRCCOMMUNICATOR_H_
#define GRCCOMMUNICATOR_H_

#include <stddef.h>
#include <vector>

#include "../core/GRCMutex.h"
#include "../core/GRCObject.h"

template<class TSESSION>
class GRCCommunicatorT: public GRCObject
{
public:
	typedef std::vector<TSESSION*> VecSession;

public:
	GRCCommunicatorT(const char* name, size_t maxSessionCount, size_t maxPacketSize)
	{
		updateObjName(name);
		for (size_t i = 0; i < maxSessionCount; ++i)
		{
			m_sessions.push_back(new TSESSION(maxPacketSize));
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
	size_t findFreeIndex()
	{
		for (size_t i = 0; i < m_sessions.size(); ++i)
		{
			if (m_sessions[i]->isActivate() == false) return i;
		}

		return GRC_INVALID_INDEX;
	}

	void closeAll()
	{
		for (size_t i = 0; i < m_sessions.size(); ++i)
		{
			m_sessions[i]->close("close all");
		}
	}

protected:
	GRCMutex m_mutex;
	VecSession m_sessions;
};

#endif /* GRCCOMMUNICATOR_H_ */
