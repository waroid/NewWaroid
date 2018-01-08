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
#include <queue>

#include "communication/GRCSerialSession.h"
#include "core/GRCBoolean.h"
#include "core/GRCCounter.h"
#include "core/GRCMutex.h"
#include "Defines.h"

namespace WAROIDCONTROLBOARD
{
	struct PACKET;
} /* namespace WAROIDCONTROLBOARD */

class ControlBoardSession: public GRCSerialSession
{
private:
	using LedQueue = std::queue<double>;

public:
	ControlBoardSession(size_t maxPacketSize);
	virtual ~ControlBoardSession();

public:
	void sendStopAll();
	void sendMove(WAROIDDIRECTION::ETYPE dir, WAROIDSPEED::ETYPE speed);
	void sendFire(bool on);
	void sendLed(bool on);

	void blinkLed(float onSeconds, float offSeconds, int count = 1);

protected:
	virtual void onOpen() override;
	virtual void onClose() override;
	virtual int onParsing(const char* data, int size, int& skipSize) override;
	virtual void onPacket(const char* packet, int size) override;

private:
	int getSkipSize(const char* data, int size);
	void sendPacket(const WAROIDCONTROLBOARD::PACKET& packet);

	void onRequestHeartbeat();
	void onProcessLed();

private:
	pthread_t m_heartbeatThread;
	GRCCounter m_requestHeartbeat;
	GRCBoolean m_green;

	pthread_t m_ledThread;
	LedQueue m_ledQueue;
	GRCMutex m_ledMutex;

	bool m_currentLed;

	WAROIDDIRECTION::ETYPE m_oldDirection;
	WAROIDSPEED::ETYPE m_oldSpeed;

private:
	static void* heartbeatWorker(void* param);
	static void* ledWorker(void* param);
};

#endif /* CONTROLBOARDSESSION_H_ */
