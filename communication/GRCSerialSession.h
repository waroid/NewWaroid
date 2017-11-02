/*
 * GRCSerialSession.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef GRCSERIALSESSION_H_
#define GRCSERIALSESSION_H_

#include <stddef.h>

#include "../core/GRCBuffer.h"
#include "GRCBaseSession.h"

class GRCSerialSession: public GRCBaseSession
{
public:
	GRCSerialSession(size_t maxPacketSize);
	virtual ~GRCSerialSession();

public:
	bool open(const char* device, int baud);

protected:
	virtual void onOpen() override;
	virtual void onClose() override;
	virtual bool onSend(const void* data, size_t size) override;
	virtual void onReceiving() override;

	virtual int onParsing(const char* data, int size, int& skipSize) = 0;
	virtual void onPacket(const char* packet, int size) = 0;

private:
	void recv(GRCBuffer& buffer);

};

#endif /* GRCSERIALSESSION_H_ */
