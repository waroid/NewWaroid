/*
 * GameSessionDefines.h
 *
 *  Created on: Oct 15, 2017
 *      Author: mirime
 */

#ifndef GAMESESSIONEFINES_H_
#define GAMESESSIONEFINES_H_

//////////
//
#define WAROID_ROBOT_GAME_MAX_PACKET_SIZE	50
#define WAROID_ROBOT_GAME_RECONNECT_SECONDS	5

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

			G_R_CAMERA,
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
		HEADER(COMMAND::ETYPE command, int packetSize)
				: 	m_packetSize(static_cast<unsigned short>(packetSize)),
					m_command(static_cast<char>(command))
		{
		}
		;

	public:
		COMMAND::ETYPE getCommand() const
		{
			return static_cast<COMMAND::ETYPE>(m_command);
		}
		int getPacketSize() const
		{
			return m_packetSize;
		}

	private:
		unsigned short m_packetSize;
		char m_command;

	public:
		static int getSize()
		{
			return sizeof(HEADER);
		}
	};

#define WAROID_ROBOT_GAME_PACKET_STRUCT_START(cmd)	struct cmd: public HEADER{cmd():HEADER(COMMAND::cmd, sizeof(*this)){}
#define WAROID_ROBOT_GAME_PACKET_STRUCT_END			}
	////////////////////
	//	packet
	WAROID_ROBOT_GAME_PACKET_STRUCT_START(HEARTBEAT_1)
		unsigned int serverTick;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(HEARTBEAT_2)
		unsigned int serverTick;
		unsigned int clientTick;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(HEARTBEAT_3)
		unsigned int clientTick;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(R_G_ACTIVE)
		int id;
		int type;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(R_G_INFO)
		float yaw;
		int battery;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(G_R_CAMERA)
		int	fps;
		int bitRate;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(G_R_ATTACHED)
		unsigned int validateKey;
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(G_R_DETACHED)
	WAROID_ROBOT_GAME_PACKET_STRUCT_END;

	WAROID_ROBOT_GAME_PACKET_STRUCT_START(G_R_UPDATE_SECOND_WEAPON)
		int weaponId;
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
