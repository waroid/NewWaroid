/*
 * ControlBoardSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "ControlBoardSession.h"

//#include <pthread.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

#include "core/GRCCore.h"
#include "core/GRCLogger.h"
//#include "CommonDefines.h"
//#include "Defines.h"
#include "Manager.h"
#include "RobotData.h"
#include "RobotInfo.h"

namespace CONTROL_BOARD_SESSION
{
	const int PACKET_SIZE = sizeof(WAROIDSERIALPACKET);
}
using namespace CONTROL_BOARD_SESSION;

ControlBoardSession::ControlBoardSession()
		: m_thread(INVALID_THREAD)
{
	// TODO Auto-generated constructor stub

}

ControlBoardSession::~ControlBoardSession()
{
	// TODO Auto-generated destructor stub
}

void ControlBoardSession::sendInitYaw()
{
	WAROIDSERIALPACKET packet;
	packet.cmd = WAROIDSERIALCOMMAND::RP_AR_INIT_YAW;
	sendPacket(packet);
}

void ControlBoardSession::sendStopAll()
{
	WAROIDSERIALPACKET packet;
	packet.cmd = WAROIDSERIALCOMMAND::RP_AR_STOP_ALL;
	sendPacket(packet);
}

void ControlBoardSession::sendMove(WAROIDROBOTDIRECTION::ETYPE dir, WAROIDROBOTSPEED::ETYPE speed)
{
	WAROIDSERIALPACKET packet;
	packet.cmd = WAROIDSERIALCOMMAND::RP_AR_MOVE;
	packet.hi = (char)dir;
	packet.low = (char)speed;
	sendPacket(packet);
}

void ControlBoardSession::sendFire(bool on)
{
	WAROIDSERIALPACKET packet;
	packet.cmd = WAROIDSERIALCOMMAND::RP_AR_FIRE;
	packet.hi = on ? 1 : 0;
	sendPacket(packet);
}

void ControlBoardSession::sendLed(bool on)
{
	WAROIDSERIALPACKET packet;
	packet.cmd = WAROIDSERIALCOMMAND::RP_AR_LED;
	packet.hi = on ? 1 : 0;
	sendPacket(packet);
}

void ControlBoardSession::onOpen()
{
	GRCSerialSession::onOpen();

	pthread_create(&m_thread, NULL, initWorker, this);
}

int ControlBoardSession::onParsing(const char* data, int size, int& skipSize)
{
	skipSize = getSkipSize(data, size);

	if (size < (skipSize + PACKET_SIZE)) return 0;

	WAROIDSERIALPACKET* packet = (WAROIDSERIALPACKET*) (data + skipSize);
	if (packet->postfix != SERIAL_POSTFIX)
	{
		skipSize++;
		return -1;
	}

	return PACKET_SIZE;
}

void ControlBoardSession::onPacket(const char* packet, int size)
{
	WAROIDSERIALPACKET* wsp = (WAROIDSERIALPACKET*) packet;
	switch (wsp->cmd)
	{
		case WAROIDSERIALCOMMAND::AR_RP_INIT_OK:
		{
			Manager::getRobotInfo().setReady();
		}
		break;
		case WAROIDSERIALCOMMAND::AR_RP_YAW:
		{
			Manager::getRobotInfo().updateYaw((int)wsp->hi << 8 | wsp->low);
		}
		break;
		case WAROIDSERIALCOMMAND::AR_RP_BATTERY:
		{
			Manager::getRobotInfo().updateBattery((int)wsp->hi << 8 | wsp->low);
		}
		break;
	}
}

int ControlBoardSession::getSkipSize(const char* data, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (data[i] == SERIAL_PREFIX)
		{
			return i;
		}
	}

	return size;
}

void ControlBoardSession::initializing()
{
	WAROIDSERIALPACKET packet;
	packet.cmd = (char)WAROIDSERIALCOMMAND::RP_AR_INIT;
	packet.hi = (char)Manager::getRobotInfo().getRobotData()->id;

	while (Manager::getRobotInfo().isReady() == false)
	{
		sendPacket(packet);

		sleep(5);
	}
}

void* ControlBoardSession::initWorker(void* param)
{
	ControlBoardSession* session = (ControlBoardSession*) param;

	GRC_LOG("[%s]start thread(%d)", session->m_name, pthread_self());
	session->initializing();
	GRC_LOG("[%s]stop thread(%d)", session->m_name, pthread_self());

	return NULL;
}
