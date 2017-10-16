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
	GRCTcpSession();
	virtual ~GRCTcpSession();

public:
	void accepted(int fd, const GRCSockAddr& localSockAddr, const GRCSockAddr& remoteSockAddr);
	bool connect(const GRCSockAddr& targetSockAddr, bool reconnect);
	void reconnect();

protected:
	virtual void onOpen() override;
	virtual void onClose() override;
	virtual void onReceiving() override;

	virtual int onParsing(const char* data, int size) = 0;
	virtual void onPacket(const char* packet, int size) = 0;

private:
	void opening();
	bool connecting();
	void receiving();

protected:
	GRCSockAddr m_localSockAddr;
	GRCSockAddr m_remoteSockAddr;
};

#endif /* GRCTCPSESSION_H_ */
