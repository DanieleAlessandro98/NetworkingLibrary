#pragma once

#include <Network/AbstractPeerManager.h>
#include "Peer.h"

class CPeerManager : public Net::CAbstractPeerManager
{
	public:
		CPeerManager() = default;
		~CPeerManager() = default;

		bool CanAcceptNewConnection() override;
		void AcceptPeer(std::shared_ptr<Net::CSocket> socket, std::shared_ptr<Net::SocketWatcher> serverWatcher) override;

		std::shared_ptr<Net::CAbstractPeer> GetFirstPeer();
};
