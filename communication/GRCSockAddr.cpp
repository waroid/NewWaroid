/*
 * GRCSockAddr.cpp
 *
 *  Created on: Oct 13, 2017
 *      Author: mirime
 */

#include "GRCSockAddr.h"

#include <pthread.h>
#include <cerrno>
#include <cstdio>

#include "../core/GRCCore.h"
#include "../core/GRCLogger.h"


GRCSockAddr::GRCSockAddr()
{
	// TODO Auto-generated constructor stub
	clear();
}

GRCSockAddr::GRCSockAddr(const char* ip, int port)
{
	// TODO Auto-generated constructor stub
	set(ip, port);
}

GRCSockAddr::GRCSockAddr(const sockaddr* sockAddr)
{
	// TODO Auto-generated constructor stub
	set(sockAddr);
}

GRCSockAddr::~GRCSockAddr()
{
	// TODO Auto-generated destructor stub
}

void GRCSockAddr::set(const char* ip, int port)
{
	clear();

	m_sockAddrIn.sin_family = AF_INET;
	m_sockAddrIn.sin_addr.s_addr = ip ? inet_addr(ip) : htonl(INADDR_ANY);
	m_sockAddrIn.sin_port = htons(port);

	m_address.format("%s:%d", inet_ntoa(m_sockAddrIn.sin_addr), ntohs(m_sockAddrIn.sin_port));
}

void GRCSockAddr::set(const sockaddr* sockAddr)
{
	clear();

	GRC_CHECK_RETURN(sockAddr);

	memcpy(&m_sockAddrIn, sockAddr, sizeof(sockaddr));

	m_address.format("%s:%d", inet_ntoa(m_sockAddrIn.sin_addr), ntohs(m_sockAddrIn.sin_port));
}

void GRCSockAddr::clear()
{
	bzero(&m_sockAddrIn, sizeof(m_sockAddrIn));
	m_address.clear();
}
