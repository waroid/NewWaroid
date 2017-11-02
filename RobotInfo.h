/*
 * RobotInfo.h
 *
 *  Created on: Oct 16, 2017
 *      Author: mirime
 */

#ifndef ROBOTINFO_H_
#define ROBOTINFO_H_

#include "RobotData.h"
#include "WeaponData.h"

class RobotInfo
{
public:
	RobotInfo();
	virtual ~RobotInfo();

public:
	bool init(int id, const char* typeName);

	void updateSecondWeapon(int weaponId);
	void updateValidateKey(unsigned int validateKey)
	{
		m_validateKey = validateKey;
	}

	int getId() const
	{
		return m_id;
	}

	const RobotData::DATA* getRobotData() const
	{
		return m_robotData;
	}
	const WeaponData::DATA* getFirstWeaponData() const
	{
		return m_firstWeaponData;
	}
	const WeaponData::DATA* getSecondWeaponData() const
	{
		return m_secondWeaponData;
	}

	void setReady()
	{
		m_ready = true;
	}
	;
	bool isReady() const
	{
		return m_ready;
	}

	void updateYaw(int yaw)
	{
		m_yaw = yaw;
	}
	int getYaw() const
	{
		return m_yaw;
	}

	void updateBattery(int battery)
	{
		m_battery = battery;
	}
	int getBattery() const
	{
		return m_battery;
	}

private:
	int m_id;
	const RobotData::DATA* m_robotData;
	const WeaponData::DATA* m_firstWeaponData;
	const WeaponData::DATA* m_secondWeaponData;
	bool m_ready;
	int m_yaw;
	int m_battery;
	unsigned int m_validateKey;

};

#endif /* ROBOTINFO_H_ */
