/*
 * Defines.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#include "core/GRCString.h"

//////////////////////////////////////////////////////////////////////////
//
#define CAMERA_USER_PORT	5001
#define USER_ROBOT_PORT		5002
#define ROBOT_GAME_PORT		23001

#ifdef __RPI__
#define CONTROL_BOARD_DEVICE	"/dev/ttyS0"
#else
#define CONTROL_BOARD_DEVICE	"/home/mirime/ttym0"
#endif
#define CONTROL_BOARD_BAUD		9600

#ifdef __RPI__
#define SOUND_DATA_DIR	"/usr/local/share/waroid/"
#define JSON_DATA_DIR	"/usr/local/share/waroid/"
#else
#define SOUND_DATA_DIR	"/home/mirime/RaspberryPi/NewWaroid/data/sound/"
#define JSON_DATA_DIR	"/home/mirime/RaspberryPi/NewWaroid/data/json/"
#endif

#define BOOT_SOUND_FILENAME	"Common_Booting_Sound.wav"

#define MIN_MOVE_POWER	60
#define MAX_MOVE_POWER	250

#define OBSERVER_ROBOT_TYPE		0
#define START_SECOND_WEAPON_ID	10001

//////////////////////////////////////////////////////////////////////////
//
namespace WAROIDTEAM
{
	enum ETYPE
	{
		NONE = 0,
		//-----
		RED,
		BLUE,
		//-----
		TOTAL,
	};
}

namespace WAROIDDIRECTION
{
	enum ETYPE
	{
		NONE = 0,
		//-----
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		LEFT_FORWARD,
		RIGHT_FORWARD,
		LEFT_BACKWARD,
		RIGHT_BACKWARD,
		LEFT_TURN,
		RIGHT_TURN,
		//-----
		TOTAL
	};

	extern ETYPE getType(GRCCSTR name);
}

namespace WAROIDSPEED
{
	enum ETYPE
	{
		NONE = 0,
		//-----
		SLOW,
		DEFAULT,
		FAST,
		//-----
		TOTAL
	};
}

namespace WAROIDWEAPONFIRE
{
	enum ETYPE
	{
		UNKNOWN = 0,
		//-----
		GATLING,
		CANNON,
	};

	extern ETYPE getType(GRCCSTR name);
}

#endif /* DEFINES_H_ */
