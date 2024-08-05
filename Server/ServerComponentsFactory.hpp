#pragma once

#include <Network/AbstractComponentsFactory.h>
#include "ServerPacketManager.h"

using ServerPacketManagerType = CServerPacketManager;
using ServerPeerManagerType = CPeerManager;

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
};
