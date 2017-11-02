/*
 * GRCConnectSession.h
 *
 *  Created on: Nov 2, 2017
 *      Author: mirime
 */

#ifndef COMMUNICATION_GRCCONNECTSESSION_H_
#define COMMUNICATION_GRCCONNECTSESSION_H_

#include <stddef.h>

#include "GRCBaseSession.h"
#include "GRCSockAddr.h"
#include "GRCTcpConnector.h"
#include "GRCTcpSession.h"

class GRCConnectSession : public GRCTcpSession
{
	template<class TCONNECTSESSION>
	friend class GRCTcpConnectorT;

public:
	GRCConnectSession(size_t maxPacketSize);
	virtual ~GRCConnectSession();

public:
	virtual bool isActivate() const override
	{
		return GRCTcpSession::isActivate() || m_reconnect;
	}

	bool connect(const GRCSockAddr& targetSockAddr, bool reconnect);
	void reconnect();

protected:
	virtual void onClose() override;

private:
	bool connecting();

protected:
	bool m_reconnect;
};

#endif /* COMMUNICATION_GRCCONNECTSESSION_H_ */
