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
#include "GameServerSession.h"
#include "RobotData.h"
#include "RobotInfo.h"
#include "UserSession.h"
#include "WeaponData.h"

class Manager
{
public:
	static bool start(int robotId, int robotType, const char* gameServerIp, const char* controlBoardDevice);
	static void stop();

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

private:
	static RobotData s_robotData;
	static WeaponData s_weaponData;
	static GRCTcpListenerT<UserSession> s_userListener;
	static GRCTcpConnectorT<GameServerSession> s_gameServerConnector;
	static GRCSerialOpenerT<ControlBoardSession> s_controlBoardOpener;
	static RobotInfo s_robotInfo;
};

#endif /* MANAGER_H_ */