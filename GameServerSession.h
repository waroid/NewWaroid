/*
 * GameServerSession.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef GAMESERVERSESSION_H_
#define GAMESERVERSESSION_H_

#include "communication/GRCTcpSession.h"

class GameServerSession: public GRCTcpSession
{
public:
	GameServerSession();
	virtual ~GameServerSession();

protected:
	virtual int onParsing(const char* data, int size) override;
	virtual void onPacket(const char* packet, int size) override;
};

#endif /* GAMESERVERSESSION_H_ */
