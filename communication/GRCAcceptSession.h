/*
 * GRCAcceptSession.h
 *
 *  Created on: Nov 2, 2017
 *      Author: mirime
 */

#ifndef COMMUNICATION_GRCACCEPTSESSION_H_
#define COMMUNICATION_GRCACCEPTSESSION_H_

#include <stddef.h>

#include "GRCSockAddr.h"
#include "GRCTcpSession.h"

class GRCAcceptSession : public GRCTcpSession
{
public:
	GRCAcceptSession(size_t maxPacketSize);
	virtual ~GRCAcceptSession();

public:
	void accepted(int fd, const GRCSockAddr& localSockAddr, const GRCSockAddr& remoteSockAddr);

};

#endif /* COMMUNICATION_GRCACCEPTSESSION_H_ */
