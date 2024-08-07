#pragma once

#include <Network/AbstractPeerManager.h>
#include "Peer.h"

class CPeerManager : public Net::CAbstractPeerManager
{
	public:
		CPeerManager() = default;
		~CPeerManager() = default;

		bool CanAcceptNewConnection() override;
		void OnPeerAccepted(Net::CAbstractPeer* newPeer) override;

		std::shared_ptr<Net::CAbstractPeer> GetFirstPeer();
};
