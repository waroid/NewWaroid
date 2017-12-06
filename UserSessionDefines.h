/*
 * UserSessionDefines.h
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#ifndef USERSESSIONEFINES_H_
#define USERSESSIONEFINES_H_

#include "Defines.h"

namespace WAROIDUSERROBOT
{
	////////////////////
	//	error
	namespace PERROR
	{
		enum ETYPE
		{
			UNKNOWN = 0,
			//-----
			SUCCESS,
			EXISTS_OWNER,
			//-----
			TOTAL
		};
	}
	
	namespace COMMAND
	{
		enum ETYPE
		{
			INVALID = -1,
			//-----
			HEARTBEAT_1 = 1,
			HEARTBEAT_2,
			HEARTBEAT_3,
			
			U_R_LOGIN,
			U_R_LOGIN_ACK,
			U_R_CAMERA,
			U_R_MOVE,
			U_R_FIRE,
		};
	}
}

#pragma pack(1)
namespace WAROIDUSERROBOT
{
	////////////////////
	//	packet
	class PACKET
	{
	public:
		PACKET(COMMAND::ETYPE command, int data1, int data2) : m_command(static_cast<char>(command)), m_data1(data1), m_data2(data2) {}
	
	public:
		COMMAND::ETYPE	getCommand() const { return static_cast<COMMAND::ETYPE>(m_command); }

	protected:
		char	m_command;
		int		m_data1;
		int		m_data2;

	public:
		static int	getSize() { return sizeof(PACKET); }
	};

#define WAROID_USER_ROBOT_PACKET_CLASS_0(cmd)							class cmd : public PACKET { public: cmd():PACKET(COMMAND::cmd, 0, 0) {}; }
#define WAROID_USER_ROBOT_PACKET_CLASS_1(cmd,type,var)					class cmd : public PACKET { public: cmd(type var):PACKET(COMMAND::cmd, var, 0) {}; type get##var() const {return static_cast<type>(m_data1);};}
#define WAROID_USER_ROBOT_PACKET_CLASS_1_E(cmd,ns,var)					class cmd : public PACKET { public: cmd(ns::ETYPE var):PACKET(COMMAND::cmd, var, 0) {}; ns::ETYPE get##var() const {return static_cast<ns::ETYPE>(m_data1);};}
#define WAROID_USER_ROBOT_PACKET_CLASS_2(cmd,type1,var1,type2,var2)		class cmd : public PACKET { public: cmd(type1 var1, type2 var2):PACKET(COMMAND::cmd, var1, var2) {}; type1 get##var1() const {return static_cast<type1>(m_data1);};type2 get##var2() const {return static_cast<type2>(m_data2);};}
#define WAROID_USER_ROBOT_PACKET_CLASS_2_TE(cmd,type1,var1,ns2,var2)	class cmd : public PACKET { public: cmd(type1 var1, ns2::ETYPE var2):PACKET(COMMAND::cmd, var1, var2) {}; type1 get##var1() const {return static_cast<type1>(m_data1);};ns2::ETYPE get##var2() const {return static_cast<ns2::ETYPE>(m_data2);};}
#define WAROID_USER_ROBOT_PACKET_CLASS_2_ET(cmd,ns1,var1,type2,var2)	class cmd : public PACKET { public: cmd(ns1::ETYPE var1, type2 var2):PACKET(COMMAND::cmd, var1, var2) {}; ns1::ETYPE get##var1() const {return static_cast<ns1::ETYPE>(m_data1);};type2 get##var2() const {return static_cast<type2>(m_data2);};}
#define WAROID_USER_ROBOT_PACKET_CLASS_2_EE(cmd,ns1,var1,ns2,var2)		class cmd : public PACKET { public: cmd(ns1::ETYPE var1, ns2::ETYPE var2):PACKET(COMMAND::cmd, var1, var2) {}; ns1::ETYPE get##var1() const {return static_cast<ns1::ETYPE>(m_data1);};ns2::ETYPE get##var2() const {return static_cast<ns2::ETYPE>(m_data2);};}

	WAROID_USER_ROBOT_PACKET_CLASS_1(HEARTBEAT_1, unsigned int, ServerTicket);
	WAROID_USER_ROBOT_PACKET_CLASS_2(HEARTBEAT_2, unsigned int, ServerTicket, unsigned int, ClientTicket);
	WAROID_USER_ROBOT_PACKET_CLASS_1(HEARTBEAT_3, unsigned int, ClientTicket);
	WAROID_USER_ROBOT_PACKET_CLASS_2(U_R_LOGIN, int, Id, unsigned int, ValidateKey);
	WAROID_USER_ROBOT_PACKET_CLASS_1_E(U_R_LOGIN_ACK, PERROR, Perror);
	WAROID_USER_ROBOT_PACKET_CLASS_1(U_R_CAMERA, int, On);
	WAROID_USER_ROBOT_PACKET_CLASS_2_EE(U_R_MOVE, WAROIDDIRECTION, Direction, WAROIDSPEED, Speed);
	WAROID_USER_ROBOT_PACKET_CLASS_2(U_R_FIRE, int, WeaponIndex, int, On);
	
}
#pragma pack()

#endif /* USERSESSIONEFINES_H_ */
