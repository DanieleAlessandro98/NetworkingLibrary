#include "StdAfx.h"
#include "NetAddress.h"
#include <WS2tcpip.h>
#include "NetException.h"

namespace Net
{
	CNetAddress::CNetAddress()
	{
		Clear();
	}

	void CNetAddress::Clear()
	{
		m_sockAddrIn = {};
		m_sockAddrIn.sin_family = AF_INET;
	}

	bool CNetAddress::Set(const char* c_szAddr, int port)
	{
		if (IsIP(c_szAddr))
			SetIP(c_szAddr);
		else
		{
			if (!SetDNS(c_szAddr))
				return false;
		}

		SetPort(port);
		return true;
	}

	void CNetAddress::SetIP(const char* c_szIP)
	{
		in_addr addr;
		if (inet_pton(AF_INET, c_szIP, &addr) != 1 || addr.S_un.S_addr == INADDR_NONE)
			throw NetException("CNetAddress::SetIP - Error at inet_pton(): %ld\n", WSAGetLastError());

		m_sockAddrIn.sin_addr.s_addr = addr.S_un.S_addr;
	}

	bool CNetAddress::SetDNS(const char* c_szDNS)
	{
		struct addrinfo hints;
		struct addrinfo* result = nullptr;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		int ret = getaddrinfo(c_szDNS, nullptr, &hints, &result);
		if (ret != 0 || result == nullptr)
			return false;

		struct sockaddr_in* addr = reinterpret_cast<struct sockaddr_in*>(result->ai_addr);
		m_sockAddrIn.sin_addr = addr->sin_addr;

		freeaddrinfo(result);
		return true;
	}

	void CNetAddress::SetPort(int port)
	{
		m_sockAddrIn.sin_port = htons(port);
	}

	bool CNetAddress::IsIP(const char* c_szAddr)
	{
		in_addr addr;
		return inet_pton(AF_INET, c_szAddr, &addr) == 1;
	}

	std::string CNetAddress::GetIP() const
	{
		char ipStr[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &(m_sockAddrIn.sin_addr), ipStr, sizeof(ipStr)) == NULL)
			throw NetException("CNetAddress::GetIP - Error at inet_ntop(): %ld\n", WSAGetLastError());

		return std::string(ipStr);
	}
}
