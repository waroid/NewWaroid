/*
 * GameSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "GameSession.h"

#include "communication/GRCTcpListener.h"
#include "core/GRCCore.h"
#include "core/GRCCoreUtil.h"
#include "Manager.h"
#include "UserSession.h"

WAROID_GAME_SESSION_COMMAND_FUNC_IMPLEMENTATION(HEARTBEAT_1)
{
	WAROIDROBOTGAME::HEARTBEAT_2 spacket;
	spacket.serverTick = rpacket->serverTick;
	spacket.clientTick = GRCCoreUtil::getTickCount();
	sendPacket(&spacket);
}

WAROID_GAME_SESSION_COMMAND_FUNC_IMPLEMENTATION(HEARTBEAT_3)
{
	recvHeartbeat(rpacket->clientTick);
}

WAROID_GAME_SESSION_COMMAND_FUNC_IMPLEMENTATION(G_R_ATTACHED)
{
	Manager::getRobotInfo().updateValidateKey(rpacket->validateKey);
}

WAROID_GAME_SESSION_COMMAND_FUNC_IMPLEMENTATION(G_R_DETACHED)
{
	Manager::getRobotInfo().updateValidateKey(0);
	Manager::getRobotInfo().updateSecondWeapon(0);

	UserSession* userSession = Manager::getUserListener().getFirstOpenedSession();
	if (userSession)
	{
		userSession->close("detached robot.");
	}
}

WAROID_GAME_SESSION_COMMAND_FUNC_IMPLEMENTATION(G_R_UPDATE_SECOND_WEAPON)
{
	Manager::getRobotInfo().updateSecondWeapon(rpacket->weaponId);
}

WAROID_GAME_SESSION_COMMAND_FUNC_IMPLEMENTATION(G_R_ATTACKED)
{
}

WAROID_GAME_SESSION_COMMAND_FUNC_IMPLEMENTATION(G_R_DEATH)
{
}

WAROID_GAME_SESSION_COMMAND_FUNC_IMPLEMENTATION(G_R_REVIVE)
{
}

GameSession::GameSession(size_t maxPacketSize)
		: 	GRCTcpSession(maxPacketSize),
			m_sendInfoThread(GRC_INVALID_THREAD)
{
	// TODO Auto-generated constructor stub

}

GameSession::~GameSession()
{
	// TODO Auto-generated destructor stub
}

void GameSession::onOpen()
{
	GRCTcpSession::onOpen();

	WAROIDROBOTGAME::R_G_ACTIVE spacket;
	spacket.id = Manager::getRobotInfo().getId();
	spacket.type = Manager::getRobotInfo().getRobotData()->id;
	sendPacket(&spacket);
}

int GameSession::onParsing(const char* data, int size)
{
	GRC_CHECK_RETMINUS(data);

	if (size < WAROIDROBOTGAME::HEADER::getSize()) return 0;

	const WAROIDROBOTGAME::HEADER* header = reinterpret_cast<const WAROIDROBOTGAME::HEADER*>(data);

	int packetSize = header->getPacketSize();
	if (packetSize > size) return 0;

	GRC_CHECKV_RETMINUS(packetSize <= WAROID_ROBOT_GAME_MAX_PACKET_SIZE, "over packet size. packetsize=%d", packetSize);

	return packetSize;
}

void GameSession::onPacket(const char* packet, int size)
{
	const WAROIDROBOTGAME::HEADER* rgh = (const WAROIDROBOTGAME::HEADER*)packet;

	switch (rgh->getCommand())
	{
		WAROID_GAME_SESSION_COMMAND_CASE(HEARTBEAT_1, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE(HEARTBEAT_3, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE(G_R_ATTACHED, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE(G_R_DETACHED, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE(G_R_UPDATE_SECOND_WEAPON, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE(G_R_ATTACKED, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE(G_R_DEATH, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE(G_R_REVIVE, rgh)
		default:
			GRC_LOG("invalid packet. cmd=%d", rgh->getCommand());
			break;
	}
}

void GameSession::sendPacket(const WAROIDROBOTGAME::HEADER* header)
{
	send(header, header->getPacketSize());
}

void GameSession::onSendingInfo()
{
	WAROIDROBOTGAME::R_G_INFO spacket;
	int yaw = 0;
	int battery = 0;
	unsigned char ready = 0;

	for (;;)
	{
		yaw = Manager::getRobotInfo().getYaw();
		battery = Manager::getRobotInfo().getBattery();
		ready = Manager::getRobotInfo().isReady() ? 1 : 0;
		if (yaw != spacket.yaw || battery != spacket.battery || ready != spacket.ready)
		{
			spacket.yaw = yaw;
			spacket.battery = battery;
			spacket.ready = ready;
			sendPacket(&spacket);
		}

		::usleep(100000);
	}
}

void* GameSession::sendInfoWorker(void* param)
{
	GameSession* session = (GameSession*)param;

	GRC_LOG("[%s]start send info thread(0x%x)", session->getObjName(), pthread_self());
	session->onSendingInfo();
	GRC_LOG("[%s]stop send info thread(0x%x)", session->getObjName(), pthread_self());

	return NULL;
}

