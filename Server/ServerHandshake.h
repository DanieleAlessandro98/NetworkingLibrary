#pragma once

#include <Network/AbstractPacketHandler.h>

class ServerHandshake : public Net::CAbstractPacketServerHandler
{
	public:
		ServerHandshake() = default;
		~ServerHandshake() = default;

		bool Analyze(Net::CAbstractPeer* peer, Net::TPacketHeader header) override;
};
