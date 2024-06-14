#include "StdAfx.h"
#include "Client.h"
#include <iostream>

using namespace Net;

Client::Client()
{
	isConnected = false;
	m_connectLimitTime = 0;
}

bool Client::Initialize(const char* c_szAddr, int port)
{
	try
	{
		if (!connectSocket.Create())
		{
			std::cerr << "Failed to create socket" << std::endl;
			return false;
		}

		CNetAddress netAddress;
		if (!netAddress.Set("localhost", 8080))
		{
			std::cerr << "Failed to set address" << std::endl;
			return false;
		}

		u_long arg = 1;
		ioctlsocket(connectSocket.GetSocket(), FIONBIO, &arg);	// Non-blocking mode

		if (!connectSocket.Connect(netAddress))
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

	dataStream = std::make_unique< CDataStream>(connectSocket);
	m_connectLimitTime = time(NULL) + 3;
	std::cout << "socket connected to the server" << std::endl;
	return true;
}

void Client::Process()
{
	fd_set writefds;
	FD_ZERO(&writefds);
	FD_SET(connectSocket.GetSocket(), &writefds);

	// Set timeout (optional)
	timeval timeout;
	timeout.tv_sec = 0; // 0 second timeout (immediatly)
	timeout.tv_usec = 0;

	int result = select(0, NULL, &writefds, NULL, &timeout);
	if (result == SOCKET_ERROR)
	{
		std::cerr << "select failed with error: " << WSAGetLastError() << std::endl;
		return;
	}

	if (!isConnected)
	{
		if (FD_ISSET(connectSocket.GetSocket(), &writefds))
		{
			isConnected = true;
			std::cout << "OnConnectSuccess" << std::endl;
		}
		else if (time(NULL) > m_connectLimitTime)
		{
			std::cout << "OnConnectFailed" << std::endl;
		}

		return;
	}

	if (FD_ISSET(connectSocket.GetSocket(), &writefds) && (dataStream->GetSendBufInputPos() > dataStream->GetSendBufOutpusPos()))
	{
		if (!dataStream->ProcessSend())
		{
			int error = WSAGetLastError();

			if (error != WSAEWOULDBLOCK)
			{
				std::cout << "OnRemoteDisconnect" << std::endl;
				return;
			}
		}
	}
}

bool Client::IsConnected()
{
	return isConnected;
}

void Client::TestSend()
{
	TPacketAction1 action1;
	action1.numIntero = 5;
	if (!dataStream->Send(sizeof(action1), &action1))
	{
		std::cerr << "action1 not sended" << std::endl;
		return;
	}

	std::cout << "action1 sended" << std::endl;
}
