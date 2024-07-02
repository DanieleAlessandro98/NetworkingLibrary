#pragma once

#include "Network/Socket.h"
#include "Network/SocketWatcher.h"
#include <Network/PacketDefinition.h>
#include <memory>

class Server
{
	public:
		bool Initialize(const char* c_szAddr, int port);
		void Process();
		void HandleNewConnection();

		bool ProcessRecv(std::shared_ptr<Net::CSocket> clientSocket);
		bool OnProcessRecv(std::shared_ptr<Net::CSocket> clientSocket);
		bool CheckPacket(std::shared_ptr<Net::CSocket> clientSocket, Net::TPacketHeader * packetHeader);
		void RecvErrorPacket(std::shared_ptr<Net::CSocket> clientSocket, int header);

		bool TestRecv(std::shared_ptr<Net::CSocket> clientSocket);
		bool TestSend(std::shared_ptr<Net::CSocket> clientSocket);
		bool TestAction3Recv(std::shared_ptr<Net::CSocket> clientSocket);

	private:
		Net::CSocket listenSocket;
		std::unique_ptr<Net::SocketWatcher> watcher;
};
