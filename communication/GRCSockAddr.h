/*
 * GRCSockAddr.h
 *
 *  Created on: Oct 13, 2017
 *      Author: mirime
 */

#ifndef GRCSOCKADDR_H_
#define GRCSOCKADDR_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

class GRCSockAddr
{
public:
	enum DATA
	{
		LEN = sizeof(sockaddr_in),
	};

public:
	GRCSockAddr();
	GRCSockAddr(const char* ip, int port);
	GRCSockAddr(const sockaddr* sockAddr);
	virtual ~GRCSockAddr();

public:
	bool isValid() const
	{
		return m_sockAddrIn.sin_family == AF_INET;
	}

	const char* getIp() const
	{
		return inet_ntoa(m_sockAddrIn.sin_addr);
	}
	int getPort() const
	{
		return ntohs(m_sockAddrIn.sin_port);
	}

	void set(const char* ip, int port);
	void set(const sockaddr* sockAddr);
	void clear();

	operator const sockaddr*() const
	{
		return (const sockaddr*) &m_sockAddrIn;
	}

	const char* operator*() const
	{
		return m_address;
	}

	GRCSockAddr& operator= (const GRCSockAddr& sockAddr)
	{
		memcpy(&m_sockAddrIn, &sockAddr.m_sockAddrIn, sizeof(m_sockAddrIn));
		memcpy(m_address, sockAddr.m_address, sizeof(m_address));

		return *this;
	}

private:
	sockaddr_in m_sockAddrIn;
	GRCString m_address;
};

#endif /* GRCSOCKADDR_H_ */
