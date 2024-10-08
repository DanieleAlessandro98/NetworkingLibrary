#include "StdAfx.h"
#include "ServerMain.h"
#include <iostream>
#include <Network/PacketDefinition.h>
#include "ServerComponentsFactory.hpp"

using namespace Net;

ServerMain::ServerMain()
{
	SetComponentsFactory(CreateComponentsFactory<CServerComponentsFactory>());
}

void ServerMain::OnInitializeCompleted()
{
	m_packetManager = SetPacketManager<ServerPacketManagerType>();
	m_peerManager = SetPeerManager<ServerPeerManagerType>();

	std::cout << "Server initialize completed" << std::endl;
}

void ServerMain::OnSocketListening()
{
	std::cout << "Socket Listening..." << std::endl;
}

void ServerMain::OnConnectClient(std::shared_ptr<CSocket> newClientSocket)
{
	if (!newClientSocket)
		return;

	std::cout << "New client accepted" << std::endl;
}

void ServerMain::OnDisconnectClient(Net::CAbstractPeer* peer)
{
	if (!peer)
		return;

	std::cout << "client disconnected" << std::endl;
}

void ServerMain::DisconnectAll()
{
	if (m_peerManager)
		m_peerManager->DisconnectAll();
}

void ServerMain::DisconnectFirstPeer()
{
	if (m_peerManager)
	{
		auto firstPeer = m_peerManager->GetFirstPeer();
		if (firstPeer)
			m_peerManager->DestroyDesc(firstPeer.get());
	}
}
