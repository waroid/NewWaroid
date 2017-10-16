/*
 * UserSession.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef USERSESSION_H_
#define USERSESSION_H_

#include "communication/GRCTcpSession.h"
#include "CommonDefines.h"

class UserSession: public GRCTcpSession
{
public:
	UserSession();
	virtual ~UserSession();

protected:
	virtual int onParsing(const char* data, int size) override;
	virtual void onPacket(const char* packet, int size) override;

private:
	void move(WAROIDROBOTDIRECTION::ETYPE dir, WAROIDROBOTSPEED::ETYPE speed);
	void fire(int weaponId, bool on);
};

#endif /* USERSESSION_H_ */
