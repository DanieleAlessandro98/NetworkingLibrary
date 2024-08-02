#include "StdAfx.h"
#include "ServerMain.h"
#include <iostream>
#include <Network/PacketDefinition.h>
#include "ServerComponentsFactory.hpp"

using namespace Net;

ServerMain::ServerMain()
{
	const auto factory = std::make_shared<CServerComponentsFactory>();
	SetComponentsFactory(factory);
}

void ServerMain::OnSocketListening()
{
	m_peerManager = std::make_unique<CPeerManager>();
	std::cout << "Socket Listening..." << std::endl;
}

bool ServerMain::CanAcceptNewConnection()
{
	return m_peerManager->CanAcceptNewConnection();
}

void ServerMain::OnConnectClient(std::shared_ptr<CSocket> newClientSocket)
{
	if (!newClientSocket)
		return;

	m_peerManager->AcceptPeer(newClientSocket, watcher);
	std::cout << "new client accepted" << std::endl;
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
