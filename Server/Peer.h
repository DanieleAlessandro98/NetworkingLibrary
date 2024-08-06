#pragma once

#include <memory>
#include <Network/Socket.h>
#include <Network/SocketWatcher.h>
#include <Network/AbstractPeer.h>
#include <Network/AbstractPacketHandler.h>
#include "ServerHandshake.h"

class CPeer : public Net::CAbstractPeer
{
	public:
		CPeer(std::shared_ptr<Net::SocketWatcher> serverWatcher);
		~CPeer() = default;

		void OnSetupCompleted() override;

		void StartHandshake();
		void SendHandshake(uint32_t curTime, long delta);
		bool HandshakeProcess(uint32_t time, long delta);

	private:
		std::unique_ptr<ServerHandshake> m_packetHandlerServerHandshake;
};
