/*
 * Manager.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#include "Manager.h"

#include <pthread.h>
#include <cerrno>
#include <cstring>

#include "core/GRCCore.h"
#include "core/GRCLogger.h"
#include "Defines.h"

RobotData Manager::s_robotData;
WeaponData Manager::s_weaponData;
GRCTcpListenerT<UserSession> Manager::s_userListener("UserListener", 1);
GRCTcpConnectorT<GameServerSession> Manager::s_gameServerConnector("GameServerConnector", 1, RECONNECT_SECONDS);
GRCSerialOpenerT<ControlBoardSession> Manager::s_controlBoardOpener("ControlBoardOpener", 1);
RobotInfo Manager::s_robotInfo;

bool Manager::start(int robotId, int robotType, const char* gameServerIp, const char* controlBoardDevice)
{
	GRC_CHECK_RETFALSE(gameServerIp);

	GRC_CHECK_RETFALSE(s_robotData.load());
	GRC_CHECK_RETFALSE(s_weaponData.load());
	GRC_CHECK_RETFALSE(s_userListener.listen(USER_PORT));
	GRC_CHECK_RETFALSE(s_gameServerConnector.connect(gameServerIp, GAME_SERVER_PORT, true));
	GRC_CHECK_RETFALSE(s_controlBoardOpener.open(controlBoardDevice, CONTROL_BOARD_BAUD));

	GRC_CHECK_RETFALSE(s_robotInfo.init(robotId, robotType));

	return true;
}

void Manager::stop()
{
	s_userListener.stop();
	s_gameServerConnector.stop();
	s_controlBoardOpener.stop();
}