/*
 * Manager.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef MANAGER_H_
#define MANAGER_H_

#include "communication/GRCSerialOpener.h"
#include "communication/GRCTcpConnector.h"
#include "communication/GRCTcpListener.h"
#include "ControlBoardSession.h"
#include "GameSession.h"
#include "RobotData.h"
#include "RobotInfo.h"
#include "UserSession.h"
#include "WeaponData.h"

class Manager
{
public:
	static const RobotData& getRobotData()
	{
		return s_robotData;
	}

	static const WeaponData& getWeaponData()
	{
		return s_weaponData;
	}

	static RobotInfo& getRobotInfo()
	{
		return s_robotInfo;
	}

	static GRCTcpListenerT<UserSession>& getUserListener()
	{
		return s_userListener;
	}

	static GRCSerialOpenerT<ControlBoardSession>& getControlBoardOpener()
	{
		return s_controlBoardOpener;
	}

	static bool start(int robotId, const char* robotTypeName, const char* gameServerIp);
	static void stop();

private:
	static GRCMutex s_mutex;
	static RobotData s_robotData;
	static WeaponData s_weaponData;
	static RobotInfo s_robotInfo;
	static GRCTcpListenerT<UserSession> s_userListener;
	static GRCTcpConnectorT<GameSession> s_gameConnector;
	static GRCSerialOpenerT<ControlBoardSession> s_controlBoardOpener;
};

#endif /* MANAGER_H_ */
