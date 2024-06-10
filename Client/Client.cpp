#include "StdAfx.h"
#include "Client.h"
#include <iostream>

using namespace Net;

Client::Client()
{
	isConnected = false;
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

	isConnected = true;
	std::cout << "socket connected to the server" << std::endl;
	return true;
}

bool Client::Process()
{
	TPacketAction1 action1;
	action1.numIntero = 5;
	if (!connectSocket.Send(action1))
	{
		isConnected = false;
		return false;
	}

	std::cout << "action1 sended" << std::endl;
	return true;
}

bool Client::IsConnected()
{
	return isConnected;
}
