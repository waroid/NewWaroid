/*
 * GameSessionDefines.h
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#ifndef GAMESESSIONEFINES_H_
#define GAMESESSIONEFINES_H_

namespace WAROIDROBOTGAME
{
	namespace COMMAND
	{
		enum ETYPE
		{
			INVALID = -1,
			//-----
			HEARTBEAT_1 = 1,
			HEARTBEAT_2,
			HEARTBEAT_3,
			
			R_G_ACTIVE,
			R_G_INFO,
			
			G_R_ATTACHED,
			G_R_DETACHED,
			G_R_UPDATE_SECOND_WEAPON,
			G_R_ATTACKED,
			G_R_DEATH,
			G_R_REVIVE,
		};
	}
}

#pragma pack(1)
namespace WAROIDROBOTGAME
{
	////////////////////
	//	packet header
	class HEADER
	{
	public:
		HEADER(COMMAND::ETYPE command, UINT packetSize)
			: m_command(static_cast<char>(command))
			, m_packetSize(static_cast<USHORT>(packetSize))
		{};

	public:
		COMMAND::ETYPE	getCommand() const { return static_cast<COMMAND::ETYPE>(m_command); }
		GRCCSTR			getCommandName() const { return COMMAND::getName(m_command); }
		UINT			getPacketSize() const { return m_packetSize; }

	private:
		USHORT m_packetSize;
		char m_command;

	public:
		static UINT	getSize() { return sizeof(HEADER); }
	};

#define WAROID_ROBOT_GAME_PACKET_STRUCT_START(cmd)	struct cmd:public HEADER{cmd():HEADER(COMMAND::##cmd, sizeof(*this)){}
#define WAROID_ROBOT_GAME_PACKET_STRUCT_END			}
	////////////////////
	//	packet
	WAROID_ROBOT_GAME_PACKET_STRUCT_START(HEARTBEAT_1)
		UINT	serverTick;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(HEARTBEAT_2)
		UINT	serverTick;
		UINT	clientTick;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(HEARTBEAT_3)
		UINT	clientTick;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(R_G_ACTIVE)
		int		id;
		int		type;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(R_G_INFO)
		int		yaw;
		int		battery;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(G_R_ATTACHED)
		UINT	validateKey;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(G_R_DETACHED)
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(G_R_UPDATE_SECOND_WEAPON)
		int		weaponId;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(G_R_ATTACKED)
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(G_R_DEATH)
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(G_R_REVIVE)
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;
}
#pragma pack()

#endif /* GAMESESSIONEFINES_H_ */
