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

		bool Initialize(const char* c_szAddr, int port) override;
		void OnSocketListening() override;
		bool CanAcceptNewConnection() override;
		void OnConnectClient(std::shared_ptr<Net::CSocket> newClientSocket) override;
		void OnDisconnectClient(Net::CAbstractPeer* peer) override;
		void DisconnectAll() override;

		void DisconnectFirstPeer();

	private:
		ServerPacketManagerType* m_packetManager;
		ServerPeerManagerType* m_peerManager;
};
