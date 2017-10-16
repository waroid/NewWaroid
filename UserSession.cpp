/*
 * UserSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "UserSession.h"

#include <pthread.h>
#include <cerrno>
#include <cstring>

#include "core/GRCCore.h"
#include "core/GRCLogger.h"
#include "Manager.h"
#include "WeaponData.h"

namespace USER_SESSION
{
	const int PACKET_SIZE = sizeof(WAROIDROBOTPACKET);
}
using namespace USER_SESSION;

UserSession::UserSession()
{
	// TODO Auto-generated constructor stub

}

UserSession::~UserSession()
{
	// TODO Auto-generated destructor stub
}

int UserSession::onParsing(const char* data, int size)
{
	if (size < PACKET_SIZE) return 0;

	return PACKET_SIZE;
}

void UserSession::onPacket(const char* packet, int size)
{
	WAROIDROBOTPACKET* wrp = (WAROIDROBOTPACKET*) packet;

	switch (wrp->cmd)
	{
		case WAROIDROBOTCOMMAND::C_RP_MOVE:
			move((WAROIDROBOTDIRECTION::ETYPE) wrp->hi, (WAROIDROBOTSPEED::ETYPE) wrp->low);
		break;
		case WAROIDROBOTCOMMAND::C_RP_FIRE:
			fire(wrp->hi, wrp->low == 1);
		break;
	}
}

void UserSession::move(WAROIDROBOTDIRECTION::ETYPE dir, WAROIDROBOTSPEED::ETYPE speed)
{
	GRC_CHECK_RETURN(dir >= WAROIDROBOTDIRECTION::NONE && dir < WAROIDROBOTDIRECTION::TOTAL);
	GRC_CHECK_RETURN(speed >= WAROIDROBOTSPEED::NONE && speed < WAROIDROBOTSPEED::TOTAL);

	//send serial
}

void UserSession::fire(int weaponId, bool on)
{
	const WeaponData::DATA* data = Manager::getWeaponData().find(weaponId);
	GRC_CHECKV_RETURN(data, "invalid weaponid. id=%d", weaponId);

	if (data->secondid == 0)
	{
		//send fire to arduino
	}

	//play fire sound

}
