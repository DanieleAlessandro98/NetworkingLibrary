#pragma once

#include <Network/AbstractComponentsFactory.h>
#include "ServerPacketManager.h"

using ServerPacketManagerType = CServerPacketManager;
using ServerPeerManagerType = CPeerManager;
using ServerPeerType = CPeer;

class CServerComponentsFactory : public Net::AbstractServerComponentsFactory
{
    public:
        std::shared_ptr<Net::CAbstractPacketManager> CreatePacketManager() override
        {
            return std::make_shared<ServerPacketManagerType>();
        }

        std::unique_ptr<Net::CAbstractPeerManager> CreatePeerManager() override
        {
            return std::make_unique<ServerPeerManagerType>();
        }

        std::shared_ptr<Net::CAbstractPeer> CreatePeer(std::shared_ptr<Net::SocketWatcher> serverWatcher) override
        {
            return std::make_shared<ServerPeerType>(serverWatcher);
        }
};
