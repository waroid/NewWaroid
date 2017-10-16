/*
 * GRCSerialOpener.h
 *
 *  Created on: Oct 13, 2017
 *      Author: mirime
 */

#ifndef GRCSERIALOPENER_H_
#define GRCSERIALOPENER_H_

#include "../core/GRCCore.h"
#include "GRCCommunicator.h"

template<class TSERIALSESSION>
class GRCSerialOpenerT : public GRCCommunicatorT<TSERIALSESSION>
{
protected:
	using GRCCommunicatorT<TSERIALSESSION>::m_name;
	using GRCCommunicatorT<TSERIALSESSION>::m_mutex;
	using GRCCommunicatorT<TSERIALSESSION>::m_sessions;

public:
	GRCSerialOpenerT(const char* name, size_t maxSessionCount)
			: GRCCommunicatorT<TSERIALSESSION>(name, maxSessionCount)
	{
	}
	virtual ~GRCSerialOpenerT()
	{
	}

public:
	bool open(const char* device, int baud)
	{
		GRC_CHECK_RETFALSE(device);
		GRC_CHECK_RETFALSE(baud>0);

		GRCMutexAutoLock autoLock(&m_mutex);

		int index = this->findFreeIndex();
		GRC_CHECK_RETFALSE(index!=INVALID_INDEX);

		m_sessions[index]->open(device, baud);

		return true;
	}

	void stop()
	{
		GRC_LOG("[%s]stopping...", m_name);

		this->closeAll();
	}
};


#endif /* GRCSERIALOPENER_H_ */
