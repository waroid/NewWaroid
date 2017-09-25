/*
 * Global.h
 *
 *  Created on: Mar 3, 2015
 *      Author: mirime
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

namespace WAROIDROBOTCONTROLCOMMAND
{
	enum ETYPE
	{
		NONE = 0,
		ERROR_ACK,
		CAMERA,
		//-----
		TOTAL
	};
}

#pragma pack(1)

namespace WAROIDROBOTCONTROLERROR
{
	enum ETYPE
	{
		NONE = 0,
		EXIST_OWNER,
	};
}

struct WAROIDROBOTCONTROL
{
	signed char command;
	union
	{
		unsigned short value;
		struct
		{
			signed char value0;
			signed char value1;
		};
	};
};

#pragma pack()



#endif /* GLOBAL_H_ */
