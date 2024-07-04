#pragma once

#include "Network/AbstractClient.h"
#include "Network/Socket.h"
#include <Network/DataStream.h>
#include <Network/PacketDefinition.h>
#include "ClientPacketManager.h"
#include <memory>

class Client : public Net::CAbstractClient
{
	public:
		Client();
		~Client() = default;

		void OnSocketCreated() override;
		void OnConnect() override;
		void OnConnectFail() override;
		void OnDisconnect() override;
		bool Analyze(Net::TPacketHeader header, std::shared_ptr<Net::CSocket> socket) override;

		void TestSend();
		bool TestRecv();
		void TestSendAction3();

	private:
		int __GetRandNumber();

};
