#include "StdAfx.h"
#include "Socket.h"

namespace Net
{
	CSocket::CSocket()
	{
		this->m_Socket = INVALID_SOCKET;
	}

	CSocket::CSocket(SOCKET socket)
	{
		this->m_Socket = socket;
		dataStream = std::make_shared<CDataStream>();
	}

	bool CSocket::Create()
	{
		if (m_Socket != INVALID_SOCKET)
			return false;

		m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_Socket == INVALID_SOCKET)
			throw NetException("CSocket::Create - Error at socket(): %ld\n", WSAGetLastError());

		if (!__SetReuse())
			return false;

		dataStream = std::make_shared<CDataStream>();

		return true;
	}

	bool CSocket::Close()
	{
		if (m_Socket == INVALID_SOCKET)
			return false;

		if (closesocket(m_Socket) != 0)
			throw NetException("CSocket::Close - Error at closesocket(): %ld\n", WSAGetLastError());

		return true;
	}

	bool CSocket::Bind(CNetAddress address)
	{
		SOCKADDR_IN addrIn = address.GetAddrIn();
		if (bind(m_Socket, (SOCKADDR*)&addrIn, sizeof(addrIn)) != 0)
			throw NetException("CSocket::Bind - Error at bind(): %ld\n", WSAGetLastError());

		if (!__SetNonBlock())
			return false;

		return true;
	}

	bool CSocket::Listen(CNetAddress address, int backlog)
	{
		if (!Bind(address))
			return false;

		if (listen(m_Socket, backlog) != 0)
			throw NetException("CSocket::Listen - Error at listen(): %ld\n", WSAGetLastError());

		return true;
	}

	bool CSocket::Accept(CSocket& clientSocket)
	{
		sockaddr_in addr = {};
		int len = sizeof(sockaddr_in);

		SOCKET acceptSocket = accept(m_Socket, (sockaddr*)(&addr), &len);
		if (acceptSocket == INVALID_SOCKET)
			throw NetException("CSocket::Accept - Error at accept(): %ld\n", WSAGetLastError());

		if (!__SetNonBlock())
			return false;

		clientSocket = CSocket(acceptSocket);

		return true;
	}

	bool CSocket::Connect(CNetAddress serverAddress)
	{
		SOCKADDR_IN addrIn = serverAddress.GetAddrIn();
		if (connect(m_Socket, (SOCKADDR*)&addrIn, sizeof(addrIn)) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
				throw NetException("CSocket::Connect - Error at connect(): %ld\n", WSAGetLastError());
		}

		return true;
	}

	bool CSocket::__SetReuse()
	{
		int opt = 1;
		if (setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
			return false;

		return true;
	}

	bool CSocket::__SetNonBlock()
	{
		u_long mode = 1;

		int result = ioctlsocket(m_Socket, FIONBIO, &mode);
		if (result != NO_ERROR)
			return false;

		return true;
	}
}
