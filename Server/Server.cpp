#include "StdAfx.h"
#include "Server.h"
#include <iostream>
#include <Network/PacketDefinition.h>

using namespace Net;

Server::Server()
{
	m_packetManager = std::make_shared<CServerPacketManager>(this);
}

void Server::OnSocketListening()
{
	std::cout << "Socket Listening..." << std::endl;
}

void Server::OnConnectClient(std::shared_ptr<CSocket> client_data)
{
	if (!client_data)
		return;

	std::cout << "new client accepted" << std::endl;
}

void Server::OnDisconnectClient(std::shared_ptr<CSocket> client_data)
{
	if (!client_data)
		return;

	std::cout << "client disconnected" << std::endl;
}
