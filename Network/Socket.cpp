#include "StdAfx.h"
#include "Socket.h"

namespace Net
{
	CSocket::CSocket()
	{
		this->m_Socket = INVALID_SOCKET;
	}

	bool CSocket::Create()
	{
		if (m_Socket != INVALID_SOCKET)
			return false;

		m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_Socket == INVALID_SOCKET)
			throw NetException("CSocket::Create - Error at socket(): %ld\n", WSAGetLastError());

		return true;
	}

	bool CSocket::Close()
	{
		if (m_Socket == INVALID_SOCKET)
			return false;

		if (closesocket(m_Socket) != 0)
			throw NetException("CSocket::Close - Error at socket(): %ld\n", WSAGetLastError());

		return true;
	}
}
