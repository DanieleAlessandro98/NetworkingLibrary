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

		bool AnalyzePacket(Net::TPacketHeader header) override;

		bool RecvHandshake();
		void SendHandshake(uint32_t handshake, uint32_t time, long delta);
};
