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
#include "CommonDefines.h"

class ControlBoardSession: public GRCSerialSession
{
public:
	ControlBoardSession();
	virtual ~ControlBoardSession();

public:
	void sendInitYaw();
	void sendStopAll();
	void sendMove(WAROIDROBOTDIRECTION::ETYPE dir, WAROIDROBOTSPEED::ETYPE speed);
	void sendFire(bool on);
	void sendLed(bool on);

protected:
	virtual void onOpen() override;
	virtual int onParsing(const char* data, int size, int& skipSize) override;
	virtual void onPacket(const char* packet, int size) override;

private:
	int getSkipSize(const char* data, int size);
	void initializing();

	void sendPacket(const WAROIDSERIALPACKET& packet)
	{
		send((const unsigned char*)&packet, sizeof(packet));
	}

private:
	pthread_t m_thread;

private:
	static void* initWorker(void* param);
};

#endif /* CONTROLBOARDSESSION_H_ */
