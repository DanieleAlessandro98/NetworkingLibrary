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
};
