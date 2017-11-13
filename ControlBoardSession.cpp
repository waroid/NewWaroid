/*
 * ControlBoardSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "ControlBoardSession.h"

#include <stddef.h>
#include <unistd.h>

#include "core/GRCCore.h"
#include "core/GRCObject.h"
#include "Manager.h"
#include "RobotData.h"
#include "RobotInfo.h"

namespace CONTROL_BOARD_SESSION
{
	const int PACKET_SIZE = sizeof(WAROIDCONTROLBOARD::PACKET);
}
using namespace CONTROL_BOARD_SESSION;

ControlBoardSession::ControlBoardSession(size_t maxPacketSize)
		: 	GRCSerialSession(maxPacketSize),
			m_requestInfothread(GRC_INVALID_THREAD)
{
	// TODO Auto-generated constructor stub

}

ControlBoardSession::~ControlBoardSession()
{
	// TODO Auto-generated destructor stub
}

void ControlBoardSession::sendInitYaw()
{
	WAROIDCONTROLBOARD::PACKET packet;
	packet.cmd = WAROIDCONTROLBOARD::COMMAND::RP_AR_INIT_YAW;
	sendPacket(packet);
}

void ControlBoardSession::sendStopAll()
{
	WAROIDCONTROLBOARD::PACKET packet;
	packet.cmd = WAROIDCONTROLBOARD::COMMAND::RP_AR_STOP_ALL;
	sendPacket(packet);
}

void ControlBoardSession::sendMove(WAROIDDIRECTION::ETYPE dir, WAROIDSPEED::ETYPE speed)
{
	WAROIDCONTROLBOARD::PACKET packet;
	packet.cmd = WAROIDCONTROLBOARD::COMMAND::RP_AR_MOVE;
	packet.hi = (char)dir;
	packet.low = (char)speed;
	sendPacket(packet);
}

void ControlBoardSession::sendFire(bool on)
{
	WAROIDCONTROLBOARD::PACKET packet;
	packet.cmd = WAROIDCONTROLBOARD::COMMAND::RP_AR_FIRE;
	packet.hi = on ? 1 : 0;
	sendPacket(packet);
}

void ControlBoardSession::sendLed(bool on)
{
	WAROIDCONTROLBOARD::PACKET packet;
	packet.cmd = WAROIDCONTROLBOARD::COMMAND::RP_AR_LED;
	packet.hi = on ? 1 : 0;
	sendPacket(packet);
}

void ControlBoardSession::onOpen()
{
	GRCSerialSession::onOpen();

	pthread_create(&m_requestInfothread, NULL, requestInitWorker, this);
}

void ControlBoardSession::onClose()
{
	if (m_requestInfothread != GRC_INVALID_THREAD)
	{
		pthread_cancel(m_requestInfothread);
		GRC_DEV("[%s]cancel request info thread.", getObjName());
	}

	GRCSerialSession::onClose();
}

int ControlBoardSession::onParsing(const char* data, int size, int& skipSize)
{
	skipSize = getSkipSize(data, size);

	if (size < (skipSize + PACKET_SIZE)) return 0;

	WAROIDCONTROLBOARD::PACKET* packet = (WAROIDCONTROLBOARD::PACKET*)(data + skipSize);
	if (packet->postfix != WAROID_CONTROLBOARD_POSTFIX)
	{
		skipSize++;
		return -1;
	}

	return PACKET_SIZE;
}

void ControlBoardSession::onPacket(const char* packet, int size)
{
	WAROIDCONTROLBOARD::PACKET* cbp = (WAROIDCONTROLBOARD::PACKET*)packet;
	switch (cbp->cmd)
	{
		case WAROIDCONTROLBOARD::COMMAND::AR_RP_INIT_OK:
			Manager::getRobotInfo().setReady();
			GRC_INFO("[%s]received. cmd=WAROIDCONTROLBOARD::AR_RP_INIT_OK hi=%d low=%d", getObjName(), cbp->hi, cbp->low);
			break;
		case WAROIDCONTROLBOARD::COMMAND::AR_RP_YAW:
			Manager::getRobotInfo().updateYaw((int)cbp->hi << 8 | cbp->low);
			GRC_INFO_COUNT(3, "[%s]received. cmd=WAROIDCONTROLBOARD::AR_RP_YAW hi=%d low=%d", getObjName(), cbp->hi, cbp->low);
			break;
		case WAROIDCONTROLBOARD::COMMAND::AR_RP_BATTERY:
			Manager::getRobotInfo().updateBattery((int)cbp->hi << 8 | cbp->low);
			GRC_INFO_COUNT(3, "[%s]received. cmd=WAROIDCONTROLBOARD::AR_RP_BATTERY hi=%d low=%d", getObjName(), cbp->hi, cbp->low);
			break;
		default:
			GRC_ERR("invalid packet. cmd=WAROIDCONTROLBOARD::%d hi=%d low=%d", cbp->cmd, cbp->hi, cbp->low);
			break;
	}
}

int ControlBoardSession::getSkipSize(const char* data, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (data[i] == WAROID_CONTROLBOARD_PREFIX)
		{
			return i;
		}
	}

	return size;
}

void ControlBoardSession::sendPacket(const WAROIDCONTROLBOARD::PACKET& packet)
{
	send(&packet, sizeof(packet));
}

void ControlBoardSession::onRequestinginit()
{
	WAROIDCONTROLBOARD::PACKET packet;
	packet.cmd = (char)WAROIDCONTROLBOARD::COMMAND::RP_AR_INIT;
	packet.hi = (char)Manager::getRobotInfo().getRobotData()->id;

	while (Manager::getRobotInfo().isReady() == false)
	{
		sendPacket(packet);

		sleep(5);
	}
}

void* ControlBoardSession::requestInitWorker(void* param)
{
	ControlBoardSession* session = (ControlBoardSession*)param;

	GRC_INFO("[%s]start request init thread(0x%x)", session->getObjName(), pthread_self());
	session->onRequestinginit();
	GRC_INFO("[%s]stop request init thread(0x%x)", session->getObjName(), pthread_self());

	return NULL;
}
