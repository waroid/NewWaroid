/*
 * RobotInfo.cpp
 *
 *  Created on: Oct 16, 2017
 *      Author: mirime
 */

#include "RobotInfo.h"

#include "common/GRCSoundWorker.h"
#include "core/GRCCore.h"
#include "core/GRCMath.h"
#include "core/GRCString.h"
#include "Manager.h"

RobotInfo::RobotInfo()
		: 	m_id(0),
			m_robotData(NULL),
			m_firstWeaponData(NULL),
			m_secondWeaponData(NULL),
			m_ready(false),
			m_yaw(0),
			m_battery(0),
			m_validateKey(0),
			m_cameraFps(25),
			m_cameraBitRate(15000000),
			m_userLogined(false)
{
	// TODO Auto-generated constructor stub

}

RobotInfo::~RobotInfo()
{
	// TODO Auto-generated destructor stub
}

bool RobotInfo::init(int id, const char* typeName)
{
	m_id = id;

	m_robotData = Manager::getRobotData().find(typeName);
	GRC_CHECK_RETFALSE(m_robotData);
	memcpy(m_movePowers, m_robotData->movepowers, sizeof(m_movePowers));

	GRC_CHECK_RETFALSE(GRCSoundWorker::add(m_robotData->attackedsoundfilename, false, 1));
	GRC_CHECK_RETFALSE(GRCSoundWorker::add(m_robotData->deathsoundfilename, false, 4));
	GRC_CHECK_RETFALSE(GRCSoundWorker::add(m_robotData->revivesoundfilename, false, 5));

	m_firstWeaponData = Manager::getWeaponData().find(m_robotData->weaponname);
	GRC_CHECKV_RETFALSE(m_firstWeaponData, "invalid weapon. name=%s", *m_robotData->weaponname);
	GRC_CHECK_RETFALSE(GRCSoundWorker::add(m_firstWeaponData->soundfilename, m_firstWeaponData->isRepeat(), 3));

	GRCSoundWorker::playTts("I'm %s number %d", *m_robotData->name, m_id);

	return true;
}

void RobotInfo::updateSecondWeapon(int weaponId)
{
	if (weaponId == 0)
		m_secondWeaponData = NULL;
	else
	{
		m_secondWeaponData = Manager::getWeaponData().find(weaponId);
		GRC_CHECK_RETURN(m_secondWeaponData);
		GRC_CHECK_RETURN(GRCSoundWorker::add(m_secondWeaponData->soundfilename, m_secondWeaponData->isRepeat(), 3));
	}
}

unsigned char RobotInfo::getMovePower(WAROIDDIRECTION::ETYPE dir, WAROIDSPEED::ETYPE speed)
{
	switch (speed)
	{
		case WAROIDSPEED::SLOW:
			return GRCMath::Clamp<int>(m_movePowers[dir] / 2, MIN_MOVE_POWER, MAX_MOVE_POWER);
		case WAROIDSPEED::DEFAULT:
			return GRCMath::Clamp<int>(m_movePowers[dir], MIN_MOVE_POWER, MAX_MOVE_POWER);
		case WAROIDSPEED::FAST:
			return GRCMath::Clamp<int>(m_movePowers[dir] * 2, MIN_MOVE_POWER, MAX_MOVE_POWER);
		default:
			return 0;
	}
}
