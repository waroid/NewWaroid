/*
 * GRCSerialOpener.h
 *
 *  Created on: Oct 13, 2017
 *      Author: mirime
 */

#ifndef GRCSERIALOPENER_H_
#define GRCSERIALOPENER_H_

#include <stddef.h>

#include "../core/GRCCore.h"
#include "../core/GRCMutex.h"
#include "GRCCommunicator.h"

template<class TSERIALSESSION>
class GRCSerialOpenerT: public GRCCommunicatorT<TSERIALSESSION>
{
protected:
	using GRCCommunicatorT<TSERIALSESSION>::m_mutex;
	using GRCCommunicatorT<TSERIALSESSION>::m_sessions;

public:
	GRCSerialOpenerT(const char* name, size_t maxSessionCount, size_t maxPacketSize)
			: GRCCommunicatorT<TSERIALSESSION>(name, maxSessionCount, maxPacketSize)
	{
	}
	virtual ~GRCSerialOpenerT()
	{
	}

public:
	bool open(const char* device, int baud)
	{
		GRC_CHECK_RETFALSE(device);
		GRC_CHECK_RETFALSE(baud > 0);

		GRCMutexAutoLock autoLock(&m_mutex);

		size_t index = this->findFreeIndex();
		GRC_CHECK_RETFALSE(index!=GRC_INVALID_INDEX);
		GRC_CHECK_RETFALSE(m_sessions[index]->open(device, baud));

		GRC_INFO("[%s]opened. device=%s baud=%d", this->getObjName(), device, baud);

		return true;
	}

	void stop()
	{
		GRC_INFO("[%s]stopping...", this->getObjName());

		this->closeAll();
	}
};

#endif /* GRCSERIALOPENER_H_ */
