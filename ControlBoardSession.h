/*
 * ControlBoardSession.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef CONTROLBOARDSESSION_H_
#define CONTROLBOARDSESSION_H_

#include <pthread.h>
#include <stddef.h>

#include "communication/GRCSerialSession.h"
#include "core/GRCBoolean.h"
#include "Defines.h"
#include "ControlBoardSessionDefines.h"


class ControlBoardSession: public GRCSerialSession
{
public:
	ControlBoardSession(size_t maxPacketSize);
	virtual ~ControlBoardSession();

public:
	void sendStopAll();
	void sendMove(WAROIDDIRECTION::ETYPE dir, WAROIDSPEED::ETYPE speed);
	void sendFire(bool on);
	void sendLed(bool on);

protected:
	virtual void onOpen() override;
	virtual void onClose() override;
	virtual int onParsing(const char* data, int size, int& skipSize) override;
	virtual void onPacket(const char* packet, int size) override;

private:
	int getSkipSize(const char* data, int size);
	void sendPacket(const WAROIDCONTROLBOARD::PACKET& packet);

	void onRequestHeartbeat();

private:
	pthread_t m_heartbeatThread;
	GRCBoolean m_requestHeartbeat;

private:
	static void* heartbeatWorker(void* param);
};

#endif /* CONTROLBOARDSESSION_H_ */
