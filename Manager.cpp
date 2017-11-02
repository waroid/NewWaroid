/*
 * Manager.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#include "Manager.h"

#include "core/GRCCore.h"
#include "core/GRCMutex.h"
#include "Defines.h"

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

	GRC_CHECK_RETFALSE(s_robotData.load());
	GRC_CHECK_RETFALSE(s_weaponData.load());
	GRC_CHECK_RETFALSE(s_robotInfo.init(robotId, robotTypeName));

	s_gameConnector.start();
	GRC_CHECK_RETFALSE(s_userListener.listen(USER_PORT));
#ifdef __RPI__
	GRC_CHECK_RETFALSE(s_controlBoardOpener.open(CONTROL_BOARD_DEVICE, CONTROL_BOARD_BAUD));
#endif

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
