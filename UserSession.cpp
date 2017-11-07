/*
 * UserSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "UserSession.h"

#include <stddef.h>

#include "core/GRCCore.h"
#include "Defines.h"
#include "Manager.h"
#include "RobotInfo.h"

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

	WAROIDUSERROBOT::U_R_LOGIN_ACK spacket(WAROIDUSERROBOT::PERROR::SUCCESS);
	sendPacket(spacket);
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_CAMERA)
{
	GRC_CHECK_RETURN(m_logined);

	int width = 1280;
	int height = 720;
	int fps = 25;
	int bitrate = 15000000;

	char command[256] = { 0 };
	sprintf(command, "raspivid -o - -t 0 -w %d -h %d -fps %d -b %d -hf -n | nc %s %d &", width, height, fps, bitrate, m_remoteSockAddr.getIp(), CAMERA_PORT);

#ifdef __RPI__
	system("killall raspivid");
	system("killall nc");

	system(command);
#endif
	GRC_INFO("opend camera. system=%s", command);
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

	switch (rpacket->getWeaponIndex())
	{
		case 0:
		{
			const WeaponData::DATA* weaponData = Manager::getRobotInfo().getFirstWeaponData();
			GRC_CHECK_RETURN(weaponData);
			Manager::getControlBoardOpener().getFirstOpenedSession()->sendFire(rpacket->getOn() == 1);

			//play sound;
		}
			break;

		case 1:
		{
			const WeaponData::DATA* weaponData = Manager::getRobotInfo().getSecondWeaponData();
			GRC_CHECK_RETURN(weaponData);

			//play sound;
		}
			break;
	}

	GRC_INFO("fire. weapon=%d on=%d", rpacket->getWeaponIndex(), rpacket->getOn());
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
	system("killall raspivid");
	system("killall nc");

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
		WAROID_USER_SESSION_COMMAND_CASE(HEARTBEAT_2, urp)
		WAROID_USER_SESSION_COMMAND_CASE(U_R_LOGIN, urp)
		WAROID_USER_SESSION_COMMAND_CASE(U_R_CAMERA, urp)
		WAROID_USER_SESSION_COMMAND_CASE(U_R_MOVE, urp)
		WAROID_USER_SESSION_COMMAND_CASE(U_R_FIRE, urp)
		default:
		{
			GRC_ERR("invalid packet. cmd=%d", urp->getCommand());
		}
			break;
	}
}

void UserSession::sendPacket(const WAROIDUSERROBOT::PACKET& packet)
{
	send(&packet, WAROIDUSERROBOT::PACKET::getSize());
}

