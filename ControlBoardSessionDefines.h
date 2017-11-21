/*
 * ControlBoardSessionDefines.h
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#ifndef CONTROLBOARDSESSIONDEFINES_H_
#define CONTROLBOARDSESSIONDEFINES_H_

//////////////////////////////////////////////////////////////////////////
//
#define WAROID_CONTROLBOARD_PREFIX	0xF1
#define WAROID_CONTROLBOARD_POSTFIX	0xF2
#define WAROID_IS_PREFIX(d)		((unsigned char)d == WAROID_CONTROLBOARD_PREFIX)
#define WAROID_IS_POSTFIX(d)	((unsigned char)d == WAROID_CONTROLBOARD_POSTFIX)

#pragma pack(1)
namespace WAROIDCONTROLBOARD
{
	////////////////////
	//	packet
	struct PACKET
	{
		unsigned char prefix;
		char cmd;
		char hi;
		char low;
		unsigned char postfix;

		PACKET()
				: 	prefix(WAROID_CONTROLBOARD_PREFIX),
					cmd(0),
					hi(0),
					low(0),
					postfix(WAROID_CONTROLBOARD_POSTFIX)
		{
		}
	};
}
#pragma pack()

namespace WAROIDCONTROLBOARD
{
	namespace COMMAND
	{
		enum ETYPE
		{
			NONE = 0,
			//-----
			RP_AR_HEARTBEAT = 0x11,
			RP_AR_STOP_ALL,
			RP_AR_MOVE,
			RP_AR_FIRE,
			RP_AR_LED,
			//-----
			AR_RP_HEARTBEAT_ACK = 0x21,
			AR_RP_YAW,
			AR_RP_BATTERY,
		//-----
		};
	}
}

#endif /* CONTROLBOARDSESSIONDEFINES_H_ */
