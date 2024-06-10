#include "StdAfx.h"
#include "Server.h"
#include <iostream>

using namespace Net;

bool Server::Initialize(const char* c_szAddr, int port)
{
	try
	{
		if (!listenSocket.Create())
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

		unsigned long val = 1;
		ioctlsocket(listenSocket.GetSocket(), FIONBIO, &val);

		if (!listenSocket.Listen(netAddress))
		{
			std::cerr << "Failed to listen socket" << std::endl;
			return false;
		}
	}
	catch (NetException ex)
	{
		std::cerr << "Exception: " << ex.what() << std::endl;
		return false;
	}

	std::cout << "Socket Listening..." << std::endl;
	return true;
}

void Server::Process()
{
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(listenSocket.GetSocket(), &readfds);

	// Set timeout (optional)
	timeval timeout;
	timeout.tv_sec = 1; // 1 second timeout
	timeout.tv_usec = 0;

	int result = select(0, &readfds, NULL, NULL, &timeout);
	if (result == SOCKET_ERROR)
	{
		std::cerr << "select failed with error: " << WSAGetLastError() << std::endl;
		return;
	}

	// Data is available on the listening socket
	if (FD_ISSET(listenSocket.GetSocket(), &readfds))
		HandleNewConnection(listenSocket);
}

void Server::HandleNewConnection(CSocket newClientSocket)
{
	if (!listenSocket.Accept(newClientSocket))
	{
		std::cerr << "Failed to accept new client" << std::endl;
		newClientSocket.Close();
		return;
	}

	std::cout << "new client accepted" << std::endl;
}

//while (true)
//{
//	PacketHeader header;
//	if (!newClientSocket.Recv(header))
//		break;
//
//	switch (header)
//	{
//	case PacketHeader::HEADER_ACTION1:
//	{
//		TPacketAction1 action;
//		if (!newClientSocket.Recv(action))
//			break;
//
//		std::cout << "Receved: " << "HEADER_ACTION1" << std::endl;
//		std::cout << "numIntero: " << action.numIntero << std::endl;
//	}
//	break;
//
//	case PacketHeader::HEADER_ACTION2:
//	{
//		// ...
//	}
//	break;
//	}
//}
//
//if (listenSocket.Close())
//{
//	std::cout << "socket closed" << std::endl;
//}
//else
//{
//	std::cerr << "Failed to close socket" << std::endl;
//}
