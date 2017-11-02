/*
 * GRCTcpSession.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef GRCTCPSESSION_H_
#define GRCTCPSESSION_H_

#include "GRCBaseSession.h"
#include "GRCSockAddr.h"

class GRCTcpSession: public GRCBaseSession
{
public:
	GRCTcpSession(size_t maxPacketSize);
	virtual ~GRCTcpSession();

protected:
	virtual void onOpen() override;
	virtual void onClose() override;
	virtual bool onSend(const void* data, size_t size) override;
	virtual void onReceiving() override;

	virtual int onParsing(const char* data, int size) = 0;
	virtual void onPacket(const char* packet, int size) = 0;

protected:
	GRCSockAddr m_localSockAddr;
	GRCSockAddr m_remoteSockAddr;

};

#endif /* GRCTCPSESSION_H_ */
