/*
 * UserSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "UserSession.h"

#include <stddef.h>
#include <cstdio>
#include <cstdlib>

#include "communication/GRCCommunicator.h"
#include "communication/GRCSerialOpener.h"
#include "communication/GRCSockAddr.h"
#include "core/GRCCore.h"
#include "core/GRCString.h"
#include "ControlBoardSession.h"
#include "Defines.h"
#include "Manager.h"
#include "RobotInfo.h"
#include "sound/GRCSoundWorker.h"
#include "WeaponData.h"

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(HEARTBEAT_2)
{
	this->recvHeartbeat(rpacket->getServerTicket());

	WAROIDUSERROBOT::HEARTBEAT_3 spacket(rpacket->getClientTicket());
	sendPacket(spacket);
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_LOGIN)
{
	auto eclose = [this](const char* reason)
	{
		close(reason);
	};

	GRC_CHECK_FUNC_RETURN(rpacket->getId() == Manager::getRobotInfo().getId(), eclose("invalid robot id"));
	GRC_CHECK_FUNC_RETURN(rpacket->getValidateKey() == Manager::getRobotInfo().getValidateKey(), eclose("invalid validate key"));

	m_logined = true;
	Manager::loginUser();

	WAROIDUSERROBOT::U_R_LOGIN_ACK spacket(WAROIDUSERROBOT::PERROR::SUCCESS);
	sendPacket(spacket);
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_MOVE)
{
	GRC_CHECK_RETURN(m_logined);
	GRC_CHECK_RETURN(rpacket->getDirection() >= WAROIDDIRECTION::NONE && rpacket->getDirection() < WAROIDDIRECTION::TOTAL);
	GRC_CHECK_RETURN(rpacket->getSpeed() >= WAROIDSPEED::NONE && rpacket->getSpeed() < WAROIDSPEED::TOTAL);

	//send serial
	Manager::getControlBoardOpener().getFirstOpenedSession()->sendMove(rpacket->getDirection(), rpacket->getSpeed());

	GRC_INFO("move. dir=%d speed=%d", rpacket->getDirection(), rpacket->getSpeed());
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_FIRE)
{
	GRC_CHECK_RETURN(m_logined);

	const WeaponData::DATA* weaponData = nullptr;
	switch (rpacket->getWeaponIndex())
	{
		case 0:
			weaponData = Manager::getRobotInfo().getFirstWeaponData();
			break;

		case 1:
			weaponData = Manager::getRobotInfo().getSecondWeaponData();
			break;
	}
	GRC_CHECK_RETURN(weaponData);

	if (rpacket->getOn() == 1)
	{
		if (rpacket->getWeaponIndex() == 0)
		{
			Manager::getControlBoardOpener().getFirstOpenedSession()->sendFire(true);
			if (weaponData->repeat == false)
			{
				usleep(100000);
				Manager::getControlBoardOpener().getFirstOpenedSession()->sendFire(false);
			}
		}

		GRCSoundWorker::startPlay(weaponData->soundfilename);
	}
	else
	{
		if (rpacket->getWeaponIndex() == 0)
		{
			Manager::getControlBoardOpener().getFirstOpenedSession()->sendFire(false);
		}

		if (weaponData->repeat)
		{
			GRCSoundWorker::endPlay(weaponData->soundfilename);
		}
	}

	GRC_DEV("fire. weapon=%d,%s on=%d", rpacket->getWeaponIndex(), *weaponData->name, rpacket->getOn());
}

UserSession::UserSession(size_t maxPacketSize)
		: 	GRCAcceptSession(maxPacketSize),
			m_logined(false)
{
	// TODO Auto-generated constructor stub

}

UserSession::~UserSession()
{
	// TODO Auto-generated destructor stub
}

void UserSession::onClose()
{
	m_logined = false;

	GRCAcceptSession::onClose();
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
		WAROID_USER_SESSION_COMMAND_CASE_LOG(3, HEARTBEAT_2, urp)
		WAROID_USER_SESSION_COMMAND_CASE(U_R_LOGIN, urp)
		WAROID_USER_SESSION_COMMAND_CASE_LOG(3, U_R_MOVE, urp)
		WAROID_USER_SESSION_COMMAND_CASE_LOG(3, U_R_FIRE, urp)
		default:
			GRC_ERR("invalid packet. cmd=WAROIDUSERROBOT::%d", urp->getCommand());
			break;
	}
}

void UserSession::sendPacket(const WAROIDUSERROBOT::PACKET& packet)
{
	send(&packet, WAROIDUSERROBOT::PACKET::getSize());
}

