/*
 * GameSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "GameSession.h"

#include <pthread.h>
#include <stddef.h>
#include <unistd.h>

#include "communication/GRCCommunicator.h"
#include "communication/GRCTcpListener.h"
#include "core/GRCCore.h"
#include "core/GRCCoreUtil.h"
#include "core/GRCObject.h"
#include "core/GRCString.h"
#include "Manager.h"
#include "RobotData.h"
#include "RobotInfo.h"
#include "common/GRCSoundWorker.h"
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

WAROID_GAME_SESSION_COMMAND_FUNC_IMPLEMENTATION(G_R_CAMERA)
{
	system("killall nc");

	char command[256] = { 0 };

#ifdef __RPI__
	system("killall raspivid");

	sprintf(command, "raspivid -o - -t 0 -w 1280 -h 720 -fps %d -b %d -vf -n | nc -k -l -p %d &", rpacket->fps, rpacket->bitRate, USER_CAMERA_PORT);
	system(command);
	GRC_INFO("opened camera. system=%s", command);
#else
	sprintf(command, "nc -k -l -p %d &", USER_CAMERA_PORT);
	system(command);
	GRC_INFO("opened camera. fps=%d bitrate=%d system=%s", rpacket->fps, rpacket->bitRate, command);
#endif

	GRCSoundWorker::playTts("Camera on");

	m_sendingInfo = true;
	pthread_create(&m_sendInfoThread, NULL, sendInfoWorker, this);
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
	GRCSoundWorker::startPlay(Manager::getRobotInfo().getRobotData()->attackedsoundfilename);
}

WAROID_GAME_SESSION_COMMAND_FUNC_IMPLEMENTATION(G_R_DEATH)
{
	GRCSoundWorker::startPlay(Manager::getRobotInfo().getRobotData()->deathsoundfilename);
}

WAROID_GAME_SESSION_COMMAND_FUNC_IMPLEMENTATION(G_R_REVIVE)
{
	GRCSoundWorker::startPlay(Manager::getRobotInfo().getRobotData()->revivesoundfilename);
}

GameSession::GameSession(size_t maxPacketSize)
		: 	GRCConnectSession(maxPacketSize),
			m_sendingInfo(false),
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

void GameSession::onClose()
{
	system("killall nc");
#ifdef __RPI__
	system("killall raspivid");
#endif

	m_sendingInfo = false;


	GRCTcpSession::onClose();
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
		WAROID_GAME_SESSION_COMMAND_CASE_LOG(3, HEARTBEAT_1, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE_LOG(3, HEARTBEAT_3, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE(G_R_CAMERA, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE(G_R_ATTACHED, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE(G_R_DETACHED, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE(G_R_UPDATE_SECOND_WEAPON, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE_LOG(3, G_R_ATTACKED, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE_LOG(3, G_R_DEATH, rgh)
		WAROID_GAME_SESSION_COMMAND_CASE_LOG(3, G_R_REVIVE, rgh)
		default:
			GRC_ERR("invalid packet. cmd=WAROIDROBOTGAME::%d", rgh->getCommand());
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
	float yaw = spacket.yaw = 0.0f;
	int battery = spacket.battery = 0;

	while (m_sendingInfo)
	{
		yaw = Manager::getRobotInfo().getYaw();
		battery = Manager::getRobotInfo().getBattery();
		if (yaw != spacket.yaw || battery != spacket.battery)
		{
			spacket.yaw = yaw;
			spacket.battery = battery;
			sendPacket(&spacket);

			GRC_INFO_COUNT(3, "[%s]sending info. yaw=%f battery=%d", getObjName(), yaw, battery);
		}

		GRCCoreUtil::sleep(0.1);
	}
}

void* GameSession::sendInfoWorker(void* param)
{
	GameSession* session = (GameSession*)param;

	GRC_INFO("[%s]start send info thread(0x%x)", session->getObjName(), pthread_self());
	session->onSendingInfo();
	GRC_INFO("[%s]stop send info thread(0x%x)", session->getObjName(), pthread_self());

	return NULL;
}

