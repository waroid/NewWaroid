/*
 * ControlBoardSession.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: mirime
 */

#include "ControlBoardSession.h"

#include "common/GRCSoundWorker.h"
#include "core/GRCCore.h"
#include "core/GRCCoreUtil.h"
#include "core/GRCObject.h"
#include "ControlBoardSessionDefines.h"
#include "Manager.h"
#include "RobotInfo.h"

namespace CONTROL_BOARD_SESSION
{
	const int PACKET_SIZE = sizeof(WAROIDCONTROLBOARD::PACKET);
}
using namespace CONTROL_BOARD_SESSION;

ControlBoardSession::ControlBoardSession(size_t maxPacketSize)
		: 	GRCSerialSession(maxPacketSize),
			m_heartbeatThread(GRC_INVALID_THREAD),
			m_ledThread(GRC_INVALID_THREAD)
{
	// TODO Auto-generated constructor stub

}

ControlBoardSession::~ControlBoardSession()
{
	// TODO Auto-generated destructor stub
}

void ControlBoardSession::sendStopAll()
{
	WAROIDCONTROLBOARD::PACKET packet;
	packet.cmd = WAROIDCONTROLBOARD::COMMAND::RP_AR_STOP_ALL;
	sendPacket(packet);
}

void ControlBoardSession::sendMove(WAROIDDIRECTION::ETYPE dir, WAROIDSPEED::ETYPE speed)
{
	static int values[4] = { 0, 80, 120, 160 };

	WAROIDCONTROLBOARD::PACKET packet;
	packet.cmd = WAROIDCONTROLBOARD::COMMAND::RP_AR_MOVE;
	packet.hi = (char)dir;
	packet.low = (char)values[speed];
	sendPacket(packet);
}

void ControlBoardSession::sendFire(bool on)
{
	WAROIDCONTROLBOARD::PACKET packet;
	packet.cmd = WAROIDCONTROLBOARD::COMMAND::RP_AR_FIRE;
	packet.hi = on ? 1 : 0;
	sendPacket(packet);
}

void ControlBoardSession::ledNumber(int robotId)
{
	GRCMutexAutoLock autoLock(&m_ledMutex);

	switch (robotId)
	{
		case 0:
			pushLedMosDash();
			break;
		case 1:
			pushLedMosDot();
			pushLedMosDash();
			break;
		case 2:
			pushLedMosDot();
			pushLedMosDot();
			pushLedMosDash();
			break;
		case 3:
			pushLedMosDot();
			pushLedMosDot();
			pushLedMosDot();
			pushLedMosDash();
			break;
		case 4:
			pushLedMosDot();
			pushLedMosDot();
			pushLedMosDot();
			pushLedMosDot();
			pushLedMosDash();
			break;
		case 5:
			pushLedMosDot();
			pushLedMosDot();
			pushLedMosDot();
			pushLedMosDot();
			pushLedMosDot();
			break;
		case 6:
			pushLedMosDash();
			pushLedMosDot();
			pushLedMosDot();
			pushLedMosDot();
			pushLedMosDot();
			break;
		case 7:
			pushLedMosDash();
			pushLedMosDot();
			pushLedMosDot();
			pushLedMosDot();
			break;
		case 8:
			pushLedMosDash();
			pushLedMosDot();
			pushLedMosDot();
			break;
		case 9:
			pushLedMosDash();
			pushLedMosDot();
			break;
	}
}

void ControlBoardSession::ledOK()
{
	GRCMutexAutoLock autoLock(&m_ledMutex);

	//O
	pushLedMosDash();
	pushLedMosDash();
	pushLedMosDash();

	//K
	pushLedMosDash();
	pushLedMosDot();
	pushLedMosDash();
}

void ControlBoardSession::ledSOS()
{
	GRCMutexAutoLock autoLock(&m_ledMutex);

	//S
	pushLedMosDot();
	pushLedMosDot();
	pushLedMosDot();

	//O
	pushLedMosDash();
	pushLedMosDash();
	pushLedMosDash();

	//S
	pushLedMosDot();
	pushLedMosDot();
	pushLedMosDot();
}

void ControlBoardSession::onOpen()
{
	GRCSerialSession::onOpen();

	pthread_create(&m_heartbeatThread, NULL, heartbeatWorker, this);
}

void ControlBoardSession::onClose()
{
	if (m_heartbeatThread != GRC_INVALID_THREAD)
	{
		pthread_cancel(m_heartbeatThread);
		GRC_DEV("[%s]cancel request info thread.", getObjName());
	}

	GRCSerialSession::onClose();
}

int ControlBoardSession::onParsing(const char* data, int size, int& skipSize)
{
	if (size == 0) return 0;

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
		case WAROIDCONTROLBOARD::COMMAND::AR_RP_HEARTBEAT_ACK:
			m_requestHeartbeat = false;
			if (m_green.update(true))
			{
				GRCSoundWorker::playTts("control board is green");
				sendStopAll();
				GRCCoreUtil::sleep(0.1);
				ledOK();
			}
			GRC_INFO("[%s]received. cmd=WAROIDCONTROLBOARD::AR_RP_HEARTBEAT_ACK hi=%d low=%d", getObjName(), cbp->hi, cbp->low);
			break;
		case WAROIDCONTROLBOARD::COMMAND::AR_RP_YAW:
			Manager::getRobotInfo().updateYaw(cbp->hi, cbp->low);
			GRC_INFO_COUNT(3, "[%s]received. cmd=WAROIDCONTROLBOARD::AR_RP_YAW hi=%d low=%d v=%f", getObjName(), cbp->hi, cbp->low, Manager::getRobotInfo().getYaw())
			break;
		case WAROIDCONTROLBOARD::COMMAND::AR_RP_BATTERY:
			Manager::getRobotInfo().updateBattery(cbp->hi, cbp->low);
			GRC_INFO_COUNT(3, "[%s]received. cmd=WAROIDCONTROLBOARD::AR_RP_BATTERY hi=%d low=%d v=%d", getObjName(), cbp->hi, cbp->low, Manager::getRobotInfo().getBattery())
			break;

		case WAROIDCONTROLBOARD::COMMAND::RP_AR_HEARTBEAT:
		case WAROIDCONTROLBOARD::COMMAND::RP_AR_STOP_ALL:
		case WAROIDCONTROLBOARD::COMMAND::RP_AR_MOVE:
		case WAROIDCONTROLBOARD::COMMAND::RP_AR_FIRE:
		case WAROIDCONTROLBOARD::COMMAND::RP_AR_LED:
			GRC_WARN("echo packet. cmd=WAROIDCONTROLBOARD::0x%x hi=%d low=%d", cbp->cmd, cbp->hi, cbp->low);
			break;

		default:
			GRC_ERR("invalid packet. cmd=WAROIDCONTROLBOARD::0x%x hi=%d low=%d", cbp->cmd, cbp->hi, cbp->low);
			break;
	}
}

int ControlBoardSession::getSkipSize(const char* data, int size)
{
	for (int i = 0; i < size; ++i)
	{
		//GRC_DEV("[%s]0x%x==0x%x", getObjName(), (unsigned char)data[i], WAROID_CONTROLBOARD_PREFIX);
		if (WAROID_IS_PREFIX(data[i]))
		{
			return i;
		}
	}

	return size;
}

void ControlBoardSession::pushLedMosDot()
{
	m_ledQueue.push(0.03);
	m_ledQueue.push(-0.07);
}
void ControlBoardSession::pushLedMosDash()
{
	m_ledQueue.push(0.07);
	m_ledQueue.push(-0.03);
}

void ControlBoardSession::sendPacket(const WAROIDCONTROLBOARD::PACKET& packet)
{
	send(&packet, sizeof(packet));
}

void ControlBoardSession::onRequestHeartbeat()
{
	WAROIDCONTROLBOARD::PACKET packet;
	packet.cmd = (char)WAROIDCONTROLBOARD::COMMAND::RP_AR_HEARTBEAT;

	while (true)
	{
		if (m_requestHeartbeat.update(true) == false)
		{
			m_green.update(false);

			// deactive ...
			Manager::getRobotInfo().updateBattery(0, 0);
			GRCSoundWorker::playTts("control board is red");
			ledSOS();
		}

		sendPacket(packet);

		GRCCoreUtil::sleep(10.0);
	}
}

void ControlBoardSession::onProcessLed()
{
	double value = 0;

	WAROIDCONTROLBOARD::PACKET packet;
	packet.cmd = (char)WAROIDCONTROLBOARD::COMMAND::RP_AR_LED;

	while (true)
	{
		m_ledMutex.wait();

		size_t count = m_ledQueue.size();
		for (size_t i = 0; i < count; ++i)
		{
			{
				GRCMutexAutoLock autoLock(&m_ledMutex);
				value = m_ledQueue.front();
				m_ledQueue.pop();
			}

			if (value > 0)
			{
				packet.hi = 1;
				sendPacket(packet);

				GRCCoreUtil::sleep(value);
			}
			else
			{
				packet.hi = 0;
				sendPacket(packet);

				if (value < 0) GRCCoreUtil::sleep(-value);
			}
		}
	}
}

void* ControlBoardSession::heartbeatWorker(void* param)
{
	ControlBoardSession* session = (ControlBoardSession*)param;

	GRC_INFO("[%s]start heartbeat thread(0x%x)", session->getObjName(), pthread_self());
	session->onRequestHeartbeat();
	GRC_INFO("[%s]stop heartbeat thread(0x%x)", session->getObjName(), pthread_self());

	return NULL;
}

void* ControlBoardSession::ledWorker(void* param)
{
	ControlBoardSession* session = (ControlBoardSession*)param;

	GRC_INFO("[%s]start led thread(0x%x)", session->getObjName(), pthread_self());
	session->onProcessLed();
	GRC_INFO("[%s]stop led thread(0x%x)", session->getObjName(), pthread_self());

	return NULL;
}
