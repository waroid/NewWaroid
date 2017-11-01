/*
 * RobotInfo.cpp
 *
 *  Created on: Oct 16, 2017
 *      Author: mirime
 */

#include "RobotInfo.h"

#include <stddef.h>

#include "core/GRCCore.h"
#include "Manager.h"

RobotInfo::RobotInfo()
		: m_id(0), m_robotData(NULL), m_firstWeaponData(NULL), m_secondWeaponData(NULL), m_ready(false), m_yaw(0), m_battery(0)
{
	// TODO Auto-generated constructor stub

}

RobotInfo::~RobotInfo()
{
	// TODO Auto-generated destructor stub
}

bool RobotInfo::init(int id, int type)
{
	m_id = id;

	m_robotData = Manager::getRobotData().find(type);
	GRC_CHECK_RETFALSE(m_robotData);

	m_firstWeaponData = Manager::getWeaponData().find(m_robotData->weaponname);
	GRC_CHECKV_RETFALSE(m_firstWeaponData, "invalid weapon. name=%s", m_robotData->weaponname);

	return true;
}

bool RobotInfo::equipSecondWeapon(int weaponId)
{
	m_secondWeaponData = Manager::getWeaponData().find(weaponId);
	GRC_CHECK_RETFALSE(m_secondWeaponData);

	return true;
}
