/*
 * RobotInfo.h
 *
 *  Created on: Oct 16, 2017
 *      Author: mirime
 */

#ifndef ROBOTINFO_H_
#define ROBOTINFO_H_

#include <cstring>

#include "Defines.h"
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

	unsigned int getValidateKey() const
	{
		return m_validateKey;
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

	void updateYaw(char hi, char low)
	{
		m_yaw = (static_cast<float>(hi) * 256 + low) * 0.1f;
	}
	float getYaw() const
	{
		return m_yaw;
	}

	void updateBattery(char hi, char low)
	{
		m_battery = static_cast<int>(hi) * 256 + low;
	}
	int getBattery() const
	{
		return m_battery;
	}

	void updateCamera(int fps, int bitRate)
	{
		m_cameraFps = fps;
		m_cameraBitRate = bitRate;
	}
	int getCameraFps() const
	{
		return m_cameraFps;
	}
	int getCameraBitRate() const
	{
		return m_cameraBitRate;
	}

	void updateUserLogin(bool logined)
	{
		m_userLogined = logined;
	}
	bool isUserLogin() const
	{
		return m_userLogined;
	}

	void updateMovePowers(const unsigned char* movePowers)
	{
		memcpy(m_movePowers, movePowers, sizeof(m_movePowers));
	}

	unsigned char getMovePower(WAROIDDIRECTION::ETYPE dir, WAROIDSPEED::ETYPE speed);

private:
	int m_id;
	const RobotData::DATA* m_robotData;
	const WeaponData::DATA* m_firstWeaponData;
	const WeaponData::DATA* m_secondWeaponData;
	bool m_ready;
	float m_yaw;
	int m_battery;
	unsigned int m_validateKey;
	int m_cameraFps;
	int m_cameraBitRate;
	bool m_userLogined;
	unsigned char m_movePowers[WAROIDDIRECTION::TOTAL];
};

#endif /* ROBOTINFO_H_ */
