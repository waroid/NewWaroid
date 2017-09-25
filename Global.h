/*
 * Global.h
 *
 *  Created on: Mar 3, 2015
 *      Author: mirime
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define CAMERA_PORT	5001
#define LISTEN_PORT	5002

namespace WAROIDROBOTCOMMAND
{
	enum ETYPE
	{
		NONE = 0,
        //-----
        C_R_OPEN_CAMERA,

        //-----
        R_C_ERROR,

        //-----
        TOTAL
	};
}

namespace WAROIDROBOTERROR
{
	enum ETYPE
	{
		UNKNOWN = 0,
        SUCCESS,
        EXIST_OWNER,
	};
}

#pragma pack(1)

struct WAROIDROBOTDATA
{
	unsigned char command;
	union
	{
		short data;
		struct
		{
			char data0;
			char data1;
		};
	};
};

#pragma pack()



#endif /* GLOBAL_H_ */
