/*
 * GRCSerialSession.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef GRCSERIALSESSION_H_
#define GRCSERIALSESSION_H_

#include "GRCBaseSession.h"

class GRCSerialSession: public GRCBaseSession
{
public:
	GRCSerialSession();
	virtual ~GRCSerialSession();

public:
	bool open(const char* device, int baud);
	void send(const unsigned char* data, int size);

protected:
	virtual void onReceiving() override;

	virtual void onOpen()
	{
	}

	virtual int onParsing(const char* data, int size, int& skipSize) = 0;
	virtual void onPacket(const char* packet, int size) = 0;

private:
	int recv(char* buffer, int len);
};

#endif /* GRCSERIALSESSION_H_ */
