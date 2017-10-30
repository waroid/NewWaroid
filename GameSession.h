/*
 * GameSession.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef GAMESESSION_H_
#define GAMESESSION_H_

#include "communication/GRCTcpSession.h"

class GameSession: public GRCTcpSession
{
public:
	GameSession();
	virtual ~GameSession();

protected:
	virtual int onParsing(const char* data, int size) override;
	virtual void onPacket(const char* packet, int size) override;
};

#endif /* GAMESESSION_H_ */
