#include "StdAfx.h"
#include "Client.h"
#include <iostream>
#include <random>

using namespace Net;

Client::Client()
{
	m_packetManager = std::make_shared<CClientPacketManager>(this);
}

void Client::OnSocketCreated()
{
	std::cout << "socket created" << std::endl;
}

void Client::OnConnect()
{
	std::cout << "socket connected to the server" << std::endl;
}

void Client::OnConnectFail()
{
	std::cout << "OnConnectFailed" << std::endl;
}

void Client::OnDisconnect()
{
	std::cout << "OnRemoteDisconnect" << std::endl;
}
