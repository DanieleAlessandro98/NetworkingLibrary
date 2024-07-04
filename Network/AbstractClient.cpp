#include "StdAfx.h"
#include <iostream>
#include "AbstractClient.h"

namespace Net
{
	CAbstractClient::CAbstractClient()
	{
		isConnected = false;
		m_connectLimitTime = 0;
		connectSocket = std::make_shared<CSocket>();
	}

	bool CAbstractClient::Initialize(const char* c_szAddr, int port)
	{
		if (!connectSocket)
			return false;

		try
		{
			if (!connectSocket->Create())
			{
				std::cerr << "Failed to create socket" << std::endl;
				return false;
			}

			CNetAddress netAddress;
			if (!netAddress.Set(c_szAddr, port))
			{
				std::cerr << "Failed to set address" << std::endl;
				return false;
			}

			u_long arg = 1;
			ioctlsocket(connectSocket->GetSocket(), FIONBIO, &arg);	// Non-blocking mode

			if (!connectSocket->Connect(netAddress))
			{
				std::cerr << "Failed to connect to the server" << std::endl;
				return false;
			}
		}
		catch (NetException ex)
		{
			std::cerr << "Exception: " << ex.what() << std::endl;
			return false;
		}

		m_connectLimitTime = time(NULL) + 3;
		OnSocketCreated();
		return true;
	}

	void CAbstractClient::Process()
	{
		if (!connectSocket)
			return;

		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(connectSocket->GetSocket(), &readfds);

		fd_set writefds;
		FD_ZERO(&writefds);
		FD_SET(connectSocket->GetSocket(), &writefds);

		// Set timeout (optional)
		timeval timeout;
		timeout.tv_sec = 0; // 0 second timeout (immediatly)
		timeout.tv_usec = 0;

		int result = select(0, &readfds, &writefds, NULL, &timeout);
		if (result == SOCKET_ERROR)
		{
			std::cerr << "select failed with error: " << WSAGetLastError() << std::endl;
			return;
		}

		if (!isConnected)
		{
			if (FD_ISSET(connectSocket->GetSocket(), &writefds))
			{
				isConnected = true;
				OnConnect();
			}
			else if (time(NULL) > m_connectLimitTime)
			{
				OnConnectFail();
			}

			return;
		}

		const auto dataStream = connectSocket->GetDataStream();
		if (!dataStream)
			return;

		if (FD_ISSET(connectSocket->GetSocket(), &writefds) && (dataStream->GetSendBufInputPos() > dataStream->GetSendBufOutpusPos()))
		{
			if (!dataStream->ProcessSend(connectSocket->GetSocket()))
			{
				int error = WSAGetLastError();
				if (error != WSAEWOULDBLOCK)
				{
					OnDisconnect();
					return;
				}
			}
		}

		if (FD_ISSET(connectSocket->GetSocket(), &readfds))
		{
			if (!m_packetManager->ProcessRecv(connectSocket))
			{
				OnDisconnect();
				return;
			}
		}
	}

	bool CAbstractClient::IsConnected()
	{
		return isConnected;
	}
}
