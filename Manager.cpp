/*
 * Manager.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#include "Manager.h"

#include <unistd.h>

#include "communication/GRCCommunicator.h"
#include "core/GRCCore.h"
#include "core/GRCMutex.h"
#include "Defines.h"
#include "GameSessionDefines.h"
#include "sound/GRCSoundWorker.h"
#include "UserSessionDefines.h"

GRCMutex Manager::s_mutex;
RobotData Manager::s_robotData;
WeaponData Manager::s_weaponData;
GRCTcpListenerT<UserSession> Manager::s_userListener("UserListener", 1, WAROIDUSERROBOT::PACKET::getSize());
GRCTcpConnectorT<GameSession> Manager::s_gameConnector("GameConnector", 1, WAROID_ROBOT_GAME_RECONNECT_SECONDS, WAROID_ROBOT_GAME_MAX_PACKET_SIZE);
GRCSerialOpenerT<ControlBoardSession> Manager::s_controlBoardOpener("ControlBoardOpener", 1, sizeof(WAROIDCONTROLBOARD::PACKET));
RobotInfo Manager::s_robotInfo;

bool Manager::start(int robotId, const char* robotTypeName, const char* gameServerIp)
{
	GRC_CHECK_RETFALSE(gameServerIp);

	GRC_CHECK_RETFALSE(GRCSoundWorker::add(BOOT_SOUND_FILENAME, false, 1));
	GRCSoundWorker::startPlay(BOOT_SOUND_FILENAME);
	while (GRCSoundWorker::isPlaying())
	{
		sleep(1);
	}

	GRC_CHECK_RETFALSE(s_robotData.load());
	GRC_CHECK_RETFALSE(s_weaponData.load());
	GRC_CHECK_RETFALSE(s_robotInfo.init(robotId, robotTypeName));

	s_gameConnector.start();
	GRC_CHECK_RETFALSE(s_userListener.listen(USER_PORT));
	GRC_CHECK_RETFALSE(s_controlBoardOpener.open(CONTROL_BOARD_DEVICE, CONTROL_BOARD_BAUD));
	GRC_CHECK_RETFALSE(s_gameConnector.connect(gameServerIp, GAME_SERVER_PORT, true));



	s_mutex.wait();

	return true;
}

void Manager::stop()
{
	s_mutex.signal();

	s_controlBoardOpener.stop();
	s_gameConnector.stop();
	s_userListener.stop();
}

void Manager::loginUser()
{
	GRCSoundWorker::startPlay(BOOT_SOUND_FILENAME);

	switch (s_robotInfo.getId())
	{
		case 0:
			mosDash();
			mosDash();
			mosDash();
			mosDash();
			mosDash();
			break;
		case 1:
			mosDot();
			mosDash();
			mosDash();
			mosDash();
			mosDash();
			break;
		case 2:
			mosDot();
			mosDot();
			mosDash();
			mosDash();
			mosDash();
			break;
		case 3:
			mosDot();
			mosDot();
			mosDot();
			mosDash();
			mosDash();
			break;
		case 4:
			mosDot();
			mosDot();
			mosDot();
			mosDot();
			mosDash();
			break;
		case 5:
			mosDot();
			mosDot();
			mosDot();
			mosDot();
			mosDot();
			break;
		case 6:
			mosDash();
			mosDot();
			mosDot();
			mosDot();
			mosDot();
			break;
		case 7:
			mosDash();
			mosDash();
			mosDot();
			mosDot();
			mosDot();
			break;
		case 8:
			mosDash();
			mosDash();
			mosDash();
			mosDot();
			mosDot();
			break;
		case 9:
			mosDash();
			mosDash();
			mosDash();
			mosDash();
			mosDot();
			break;
	}
}

void Manager::mosDot()
{
	s_controlBoardOpener.getFirstOpenedSession()->sendFire(true);
	s_controlBoardOpener.getFirstOpenedSession()->sendLed(true);
	usleep(300000);
	s_controlBoardOpener.getFirstOpenedSession()->sendFire(false);
	s_controlBoardOpener.getFirstOpenedSession()->sendLed(false);
	usleep(700000);
}

void Manager::mosDash()
{
	s_controlBoardOpener.getFirstOpenedSession()->sendFire(true);
	s_controlBoardOpener.getFirstOpenedSession()->sendLed(true);
	usleep(700000);
	s_controlBoardOpener.getFirstOpenedSession()->sendFire(false);
	s_controlBoardOpener.getFirstOpenedSession()->sendLed(false);
	usleep(300000);
}
