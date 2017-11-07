/*
 * GameSession.h
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#ifndef GAMESESSION_H_
#define GAMESESSION_H_

#include "communication/GRCConnectSession.h"
#include "GameSessionDefines.h"

class GameSession: public GRCConnectSession
{
#define WAROID_GAME_SESSION_COMMAND_FUNC_INTERFACE(cmd)			void on##cmd(const WAROIDROBOTGAME::cmd* rpacket);
#define WAROID_GAME_SESSION_COMMAND_FUNC_IMPLEMENTATION(cmd)	void GameSession::on##cmd(const WAROIDROBOTGAME::cmd* rpacket)
#define WAROID_GAME_SESSION_COMMAND_CASE(cmd,p)					case WAROIDROBOTGAME::COMMAND::cmd: GRC_DEV("[%s]received. cmd=WAROIDROBOTGAME::%s", getObjName(), #cmd); on##cmd(static_cast<const WAROIDROBOTGAME::cmd*>(p)); break;

private:
	WAROID_GAME_SESSION_COMMAND_FUNC_INTERFACE(HEARTBEAT_1)
	WAROID_GAME_SESSION_COMMAND_FUNC_INTERFACE(HEARTBEAT_3)
	WAROID_GAME_SESSION_COMMAND_FUNC_INTERFACE(G_R_ATTACHED)
	WAROID_GAME_SESSION_COMMAND_FUNC_INTERFACE(G_R_DETACHED)
	WAROID_GAME_SESSION_COMMAND_FUNC_INTERFACE(G_R_UPDATE_SECOND_WEAPON)
	WAROID_GAME_SESSION_COMMAND_FUNC_INTERFACE(G_R_ATTACKED)
	WAROID_GAME_SESSION_COMMAND_FUNC_INTERFACE(G_R_DEATH)
	WAROID_GAME_SESSION_COMMAND_FUNC_INTERFACE(G_R_REVIVE)

public:
	GameSession(size_t maxPacketSize);
	virtual ~GameSession();

protected:
	virtual void onOpen() override;
	virtual int onParsing(const char* data, int size) override;
	virtual void onPacket(const char* packet, int size) override;

private:
	void sendPacket(const WAROIDROBOTGAME::HEADER* header);

	void onSendingInfo();

private:
	pthread_t	m_sendInfoThread;

private:
	static void* sendInfoWorker(void* param);

};

#endif /* GAMESESSION_H_ */
