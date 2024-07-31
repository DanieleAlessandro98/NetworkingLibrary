#pragma once

#include "Network/AbstractServer.h"
#include "Network/Socket.h"
#include "Network/SocketWatcher.h"
#include <Network/PacketDefinition.h>
#include "ServerPacketManager.h"
#include <memory>
#include <Network/PacketDefinition.h>
#include <Server/PeerManager.h>

class Server : public Net::CAbstractServer
{
	public:
		Server();
		~Server() = default;

		void OnSocketListening() override;
		bool CanAcceptNewConnection() override;
		void OnConnectClient(std::shared_ptr<Net::CSocket> client_data) override;
		void OnDisconnectClient(std::shared_ptr<Net::CSocket> client_data) override;
		void DisconnectAll() override;

		void DisconnectFirstPeer();

	private:
		std::unique_ptr<CPeerManager> m_peerManager;
};
