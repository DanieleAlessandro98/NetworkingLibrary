#pragma once

#include "Network/AbstractServer.h"
#include "Network/Socket.h"
#include "Network/SocketWatcher.h"
#include <Network/PacketDefinition.h>
#include "ServerPacketManager.h"
#include <memory>
#include <Network/PacketDefinition.h>
#include <Server/PeerManager.h>
#include "ServerComponentsFactory.hpp"

class ServerMain : public Net::CAbstractServer
{
	public:
		ServerMain();
		~ServerMain() = default;

		void OnInitializeCompleted() override;
		void OnSocketListening() override;
		void OnConnectClient(std::shared_ptr<Net::CSocket> newClientSocket) override;
		void OnDisconnectClient(Net::CAbstractPeer* peer) override;
		void DisconnectAll() override;

		void DisconnectFirstPeer();

	private:
		ServerPacketManagerType* m_packetManager;
		ServerPeerManagerType* m_peerManager;
};
