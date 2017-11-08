/*
 * Defines.h
 *
 *  Created on: Oct 12, 2017
 *      Author: mirime
 */

#ifndef DEFINES_H_
#define DEFINES_H_

//////////////////////////////////////////////////////////////////////////
//
#define CAMERA_PORT				5001
#define USER_PORT				5002
#define GAME_SERVER_PORT		5003

#define MAX_CAMERA_MODE		6

#define CONTROL_BOARD_DEVICE	"/dev/ttyS0"
#define CONTROL_BOARD_BAUD		9600

#ifdef __RPI__
#define SOUND_DATA_DIR	"/usr/local/share/waroid/"
#define JSON_DATA_DIR	"/usr/local/share/waroid/"
#else
#define SOUND_DATA_DIR	"/home/mirime/RaspberryPi/NewWaroid/data/sound/"
#define JSON_DATA_DIR	"/home/mirime/RaspberryPi/NewWaroid/data/json/"
#endif

#define BOOT_SOUND_FILENAME	"Common_Booting_Sound.wav"

//////////////////////////////////////////////////////////////////////////
//
namespace WAROIDTEAM {
enum ETYPE {
	NONE = 0,
	//-----
	RED,
	BLUE,
	//-----
	TOTAL,
};
}

namespace WAROIDDIRECTION {
enum ETYPE {
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
}

namespace WAROIDSPEED {
enum ETYPE {
	NONE = 0,
	//-----
	SLOW,
	DEFAULT,
	FAST,
	//-----
	TOTAL
};
}

#endif /* DEFINES_H_ */
