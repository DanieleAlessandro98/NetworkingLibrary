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
	CSocket newClientSocket;
	if (!listenSocket.Accept(newClientSocket))
	{
		std::cerr << "Failed to accept new client" << std::endl;
		newClientSocket.Close();
	}
	else
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
