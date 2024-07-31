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
	m_peerManager = std::make_unique<CPeerManager>();
	std::cout << "Socket Listening..." << std::endl;
}

void Server::OnConnectClient(std::shared_ptr<CSocket> client_data)
{
	if (!client_data)
		return;

	m_peerManager->AcceptPeer(client_data, watcher);
	std::cout << "new client accepted" << std::endl;
}

bool Server::CanAcceptNewConnection()
{
	return m_peerManager->CanAcceptNewConnection();
}

void Server::OnDisconnectClient(std::shared_ptr<CSocket> client_data)
{
	if (!client_data)
		return;

	std::cout << "client disconnected" << std::endl;
}
