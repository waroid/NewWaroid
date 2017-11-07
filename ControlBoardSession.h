/*
 * ControlBoardSession.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef CONTROLBOARDSESSION_H_
#define CONTROLBOARDSESSION_H_

#include <pthread.h>

#include "communication/GRCSerialSession.h"
#include "Defines.h"
#include "ControlBoardSessionDefines.h"

class ControlBoardSession: public GRCSerialSession
{
public:
	ControlBoardSession(size_t maxPacketSize);
	virtual ~ControlBoardSession();

public:
	void sendInitYaw();
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

	void onRequestinginit();

private:
	pthread_t m_requestInfothread;

private:
	static void* requestInitWorker(void* param);
};

#endif /* CONTROLBOARDSESSION_H_ */
