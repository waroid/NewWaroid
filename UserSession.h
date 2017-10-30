/*
 * UserSession.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef USERSESSION_H_
#define USERSESSION_H_

#include "communication/GRCTcpSession.h"
#include "UserSessionDefines.h"

class UserSession: public GRCTcpSession
{
#define WAROID_USER_SESSION_COMMAND_FUNC_INTERFACE(cmd)			void on##cmd(const WAROIDUSERROBOT::cmd* rpacket)
#define WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(cmd)	void UserSession::on##cmd(const WAROIDUSERROBOT::cmd* rpacket)
#define WAROID_USER_SESSION_COMMAND_CASE(cmd,p)					case WAROIDUSERROBOT::COMMAND::cmd: on##cmd(static_cast<const WAROIDUSERROBOT::cmd*>(p)); break

public:
	UserSession();
	virtual ~UserSession();

protected:
	virtual int onParsing(const char* data, int size) override;
	virtual void onPacket(const char* packet, int size) override;

private:
	WAROID_USER_SESSION_COMMAND_FUNC_INTERFACE(HEARTBEAT_2);
	WAROID_USER_SESSION_COMMAND_FUNC_INTERFACE(U_R_LOGIN);
	WAROID_USER_SESSION_COMMAND_FUNC_INTERFACE(U_R_CAMERA);
	WAROID_USER_SESSION_COMMAND_FUNC_INTERFACE(U_R_MOVE);
	WAROID_USER_SESSION_COMMAND_FUNC_INTERFACE(U_R_FIRE);
};

#endif /* USERSESSION_H_ */
