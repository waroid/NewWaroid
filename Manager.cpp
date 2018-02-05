/*
 * Manager.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#include "Manager.h"

#include <unistd.h>

#include "common/GRCSoundWorker.h"
#include "communication/GRCCommunicator.h"
#include "core/GRCCore.h"
#include "core/GRCCoreUtil.h"
#include "core/GRCMutex.h"
#include "Defines.h"
#include "GameSessionDefines.h"
#include "UserSessionDefines.h"
#include "ControlBoardSessionDefines.h"

GRCMutex Manager::s_mutex;
RobotData Manager::s_robotData;
WeaponData Manager::s_weaponData;
RobotInfo Manager::s_robotInfo;
GRCTcpListenerT<UserSession> Manager::s_userListener("UserListener", 1, WAROIDUSERROBOT::PACKET::getSize());
GRCTcpConnectorT<GameSession> Manager::s_gameConnector("GameConnector", 1, WAROID_ROBOT_GAME_RECONNECT_SECONDS, WAROID_ROBOT_GAME_MAX_PACKET_SIZE);
GRCSerialOpenerT<ControlBoardSession> Manager::s_controlBoardOpener("ControlBoardOpener", 1, sizeof(WAROIDCONTROLBOARD::PACKET));

bool Manager::start(int robotId, const char* robotTypeName, const char* gameServerIp)
{
	GRC_CHECK_RETFALSE(gameServerIp);

	GRC_CHECK_RETFALSE(GRCSoundWorker::add(BOOT_SOUND_FILENAME, false, 1));
	GRCSoundWorker::startPlay(BOOT_SOUND_FILENAME);
	while (GRCSoundWorker::isPlaying())
	{
		GRCCoreUtil::sleep(0.3);
	}

	GRC_CHECK_RETFALSE(s_robotData.load());
	GRC_CHECK_RETFALSE(s_weaponData.load());
	GRC_CHECK_RETFALSE(s_robotInfo.init(robotId, robotTypeName));

	GRC_CHECK_RETFALSE(s_controlBoardOpener.open(CONTROL_BOARD_DEVICE, CONTROL_BOARD_BAUD));

	if (gameServerIp != nullptr && strcasecmp(gameServerIp, "none") != 0)
	{
		s_gameConnector.start();
		GRC_CHECK_RETFALSE(s_gameConnector.connect(gameServerIp, ROBOT_GAME_PORT, true));
	}

	GRC_CHECK_RETFALSE(s_userListener.listen(USER_ROBOT_PORT));

	s_mutex.wait();

	return true;
}

void Manager::stop()
{
	s_mutex.signal();

	s_userListener.stop();
	s_gameConnector.stop();
	s_controlBoardOpener.stop();
#ifdef __RPI__
#else
	system("killall socat");
#endif
}
