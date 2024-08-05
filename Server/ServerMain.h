#pragma once

#include "Network/AbstractServer.h"
#include "Network/Socket.h"
#include "Network/SocketWatcher.h"
#include <Network/PacketDefinition.h>
#include "ServerPacketManager.h"
#include <memory>
#include <Network/PacketDefinition.h>
#include <Server/PeerManager.h>

class ServerMain : public Net::CAbstractServer
{
	public:
		ServerMain();
		~ServerMain() = default;

		void OnSocketListening() override;
		bool CanAcceptNewConnection() override;
		void OnConnectClient(std::shared_ptr<Net::CSocket> newClientSocket) override;
		void OnDisconnectClient(Net::CAbstractPeer* peer) override;
		void DisconnectAll() override;

		void DisconnectFirstPeer();
};
