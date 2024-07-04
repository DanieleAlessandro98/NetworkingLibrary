#pragma once

#include "Network/AbstractServer.h"
#include "Network/Socket.h"
#include "Network/SocketWatcher.h"
#include <Network/PacketDefinition.h>
#include "ServerPacketManager.h"
#include <memory>
#include <Network/PacketDefinition.h>

class Server : public Net::CAbstractServer
{
	public:
		Server();
		~Server() = default;

		void OnSocketListening() override;
		void OnConnectClient(std::shared_ptr<Net::CSocket> client_data) override;
		void OnDisconnectClient(std::shared_ptr<Net::CSocket> client_data) override;
		bool Analyze(Net::TPacketHeader header, std::shared_ptr<Net::CSocket> socket) override;

		bool TestRecv(std::shared_ptr<Net::CSocket> clientSocket);
		bool TestSend(std::shared_ptr<Net::CSocket> clientSocket);
		bool TestAction3Recv(std::shared_ptr<Net::CSocket> clientSocket);
};
