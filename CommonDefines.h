/*
 * CommonDefines.h
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#ifndef COMMONDEFINES_H_
#define COMMONDEFINES_H_

//------------------------------
namespace WAROIDROBOTDIRECTION
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
}

namespace WAROIDROBOTSPEED
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

//------------------------------
//------------------------------
#pragma pack(1)

struct WAROIDROBOTPACKET
{
	char cmd;
	char hi;
	char low;
};

#pragma pack()

namespace WAROIDROBOTCOMMAND
{
	enum ETYPE
	{
		NONE = 0,
		//-----
		C_RP_MOVE,
		C_RP_FIRE,
		//-----
		RP_C_ERROR,
		//-----
		TOTAL
	};
}

namespace WAROIDROBOTERROR
{
	enum ETYPE
	{
		UNKNOWN = 0,
		//-----
		SUCCESS,
		EXIST_OWNER,
		//-----
		TOTAL
	};
}

//------------------------------
//------------------------------
#pragma pack(1)

struct WAROIDROBOTGAMEHEADER
{
	char cmd;
	unsigned short size;
};

#pragma pack()

namespace WAROIDROBOTGAMECOMMAND
{
	enum ETYPE
	{
		NONE = 0,
		//-----
		RP_GS_LOGIN,
		RP_GS_LOGIN_ACK,
		RP_GS_INFO,
		//-----
		GS_RP_ATTACKED,
		GS_RP_DEATH,
		GS_RP_REVIVE,
		//-----
		TOTAL
	};
}

#pragma pack(1)

struct RP_GS_LOGIN
{
	char id;
	char type;
};

struct RP_GS_INFO
{
	short yaw;
	char battery;
};

struct GS_RP_ATTACKED
{
};

struct GS_RP_DEATH
{
};

struct GS_RP_REVIVE
{
};

#pragma pack()

//------------------------------
//------------------------------
#define SERIAL_PREFIX	0xF1
#define SERIAL_POSTFIX	0xF2

#pragma pack(1)

struct WAROIDSERIALPACKET
{
	unsigned char prefix;
	char cmd;
	char hi;
	char low;
	unsigned char postfix;

	WAROIDSERIALPACKET()
			: prefix(SERIAL_PREFIX), cmd(0), hi(0), low(0), postfix(SERIAL_POSTFIX)
	{
	}
};

#pragma pack()

namespace WAROIDSERIALCOMMAND
{
	enum ETYPE
	{
		NONE = 0,
		//-----
		RP_AR_INIT = 0x11,
		RP_AR_INIT_YAW = 0x12,
		RP_AR_STOP_ALL = 0x13,
		RP_AR_MOVE = 0x13,
		RP_AR_FIRE = 0x14,
		RP_AR_LED = 0x15,
		//-----
		AR_RP_INIT_OK = 0x21,
		AR_RP_YAW = 0x22,
		AR_RP_BATTERY = 0x23,
		//-----
		TOTAL
	};
}

#endif /* COMMONDEFINES_H_ */
