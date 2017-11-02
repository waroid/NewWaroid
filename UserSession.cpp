/*
 * UserSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "UserSession.h"

#include "core/GRCCore.h"
#include "Defines.h"

namespace USER_SESSION
{
}
using namespace USER_SESSION;

UserSession::UserSession(size_t maxPacketSize)
		: GRCTcpSession(maxPacketSize)
{
	// TODO Auto-generated constructor stub

}

UserSession::~UserSession()
{
	// TODO Auto-generated destructor stub
}

int UserSession::onParsing(const char* data, int size)
{
	if (size < WAROIDUSERROBOT::PACKET::getSize()) return 0;

	return WAROIDUSERROBOT::PACKET::getSize();
}

void UserSession::onPacket(const char* packet, int size)
{
	const WAROIDUSERROBOT::PACKET* urp = (const WAROIDUSERROBOT::PACKET*)packet;

	switch (urp->getCommand())
	{
		WAROID_USER_SESSION_COMMAND_CASE(HEARTBEAT_2, urp)
		WAROID_USER_SESSION_COMMAND_CASE(U_R_LOGIN, urp)
		WAROID_USER_SESSION_COMMAND_CASE(U_R_CAMERA, urp)
		WAROID_USER_SESSION_COMMAND_CASE(U_R_MOVE, urp)
		WAROID_USER_SESSION_COMMAND_CASE(U_R_FIRE, urp)
		default:
		{
			GRC_LOG("invalid packet. cmd=%d", urp->getCommand());
		}
	}
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(HEARTBEAT_2)
{
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_LOGIN)
{
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_CAMERA)
{
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_MOVE)
{
	GRC_CHECK_RETURN(rpacket->getDirection() >= WAROIDDIRECTION::NONE && rpacket->getDirection() < WAROIDDIRECTION::TOTAL);
	GRC_CHECK_RETURN(rpacket->getSpeed() >= WAROIDSPEED::NONE && rpacket->getSpeed() < WAROIDSPEED::TOTAL);

	//send serial
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_FIRE)
{
	//	const WeaponData::DATA* data = Manager::getWeaponData().find(weaponId);
	//	GRC_CHECKV_RETURN(data, "invalid weaponid. id=%d", weaponId);

	//	if (data->secondid == 0)
	//	{
	//send fire to arduino
	//	}

	//play fire sound

}
