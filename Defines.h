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
#define CONTROL_BOARD_BAUD		9600

#define MAX_CAMERA_MODE		6
#define RECONNECT_SECONDS	5

#define DATA_DIR	"/usr/local/share/waroid/"

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
