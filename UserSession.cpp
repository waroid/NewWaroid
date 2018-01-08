/*
 * UserSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "UserSession.h"

#include <stddef.h>
#include <unistd.h>

#include "common/GRCSoundWorker.h"
#include "communication/GRCCommunicator.h"
#include "communication/GRCSerialOpener.h"
#include "core/GRCCore.h"
#include "core/GRCString.h"
#include "ControlBoardSession.h"
#include "Defines.h"
#include "Manager.h"
#include "RobotInfo.h"
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

#define MASTER_ID			999
#define MASTER_VALIDATEKEY	12345678

	if (rpacket->getId() == MASTER_ID)
	{
		GRC_CHECK_FUNC_RETURN(rpacket->getValidateKey() == MASTER_VALIDATEKEY, eclose("invalid validate key"));
	}
	else
	{
		GRC_CHECK_FUNC_RETURN(rpacket->getId() == Manager::getRobotInfo().getId(), eclose("invalid robot id"));
		GRC_CHECK_FUNC_RETURN(rpacket->getValidateKey() == Manager::getRobotInfo().getValidateKey(), eclose("invalid validate key"));
	}

	Manager::getRobotInfo().updateUserLogin(true);
	GRCSoundWorker::playTts("get on %s", *Manager::getRobotInfo().getRobotData()->name);
	Manager::getControlBoardOpener().getFirstOpenedSession()->blinkLed(0.2, 0.2, 5);

	WAROIDUSERROBOT::U_R_LOGIN_ACK spacket(WAROIDUSERROBOT::PERROR::SUCCESS);
	sendPacket(spacket);
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_CAMERA)
{
	GRC_CHECK_RETURN(Manager::getRobotInfo().isUserLogin());

	system("killall nc");
	system("killall raspivid");
	if (rpacket->getOn() == 1)
	{
		char command[256] = { 0 };
		sprintf(command, "raspivid -o - -t 0 -w 1280 -h 720 -fps %d -b %d -vf -n | nc %s %d &", Manager::getRobotInfo().getCameraFps(), Manager::getRobotInfo().getCameraBitRate(), m_remoteSockAddr.getIp(), CAMERA_USER_PORT);
#ifdef __RPI__
		system(command);
#endif
		GRC_INFO("opened camera. system=%s", command);
	}
	else
	{
		GRC_INFO("closed camera");
	}

	GRCSoundWorker::playTts("%s camera",
			rpacket->getOn() == 1 ? "open" : "close");
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_MOVE)
{
	GRC_CHECK_RETURN(Manager::getRobotInfo().isUserLogin());

	GRC_CHECK_RETURN(rpacket->getDirection() >= WAROIDDIRECTION::NONE && rpacket->getDirection() < WAROIDDIRECTION::TOTAL);
	GRC_CHECK_RETURN(rpacket->getSpeed() >= WAROIDSPEED::NONE && rpacket->getSpeed() < WAROIDSPEED::TOTAL);

	//send serial
	Manager::getControlBoardOpener().getFirstOpenedSession()->sendMove(rpacket->getDirection(), rpacket->getSpeed());

	GRC_DEV("[%s]move. dir=%d speed=%d", getObjName(), rpacket->getDirection(), rpacket->getSpeed());
}

WAROID_USER_SESSION_COMMAND_FUNC_IMPLEMENTATION(U_R_FIRE)
{
	GRC_CHECK_RETURN(Manager::getRobotInfo().isUserLogin());

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
			if (weaponData->isRepeat() == false)
			{
				GRCCoreUtil::sleep(0.1);
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

		if (weaponData->isRepeat())
		{
			GRCSoundWorker::endPlay(weaponData->soundfilename);
		}
	}

	GRC_DEV("[%s]fire. weapon=%d,%s on=%d", getObjName(), rpacket->getWeaponIndex(), *weaponData->name, rpacket->getOn());
}

UserSession::UserSession(size_t maxPacketSize)
		: GRCAcceptSession(maxPacketSize)
{
	// TODO Auto-generated constructor stub

}

UserSession::~UserSession()
{
	// TODO Auto-generated destructor stub
}

void UserSession::onClose()
{
	system("killall nc");
	system("killall raspivid");

	//stop all
	auto* controlBoardSession = Manager::getControlBoardOpener().getFirstOpenedSession();
	if (controlBoardSession) controlBoardSession->sendStopAll();

	if (Manager::getRobotInfo().isUserLogin())
	{
		Manager::getRobotInfo().updateUserLogin(false);
		GRCSoundWorker::playTts("alight from %s", *Manager::getRobotInfo().getRobotData()->name);
	}

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
		WAROID_USER_SESSION_COMMAND_CASE(U_R_CAMERA, urp)
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

