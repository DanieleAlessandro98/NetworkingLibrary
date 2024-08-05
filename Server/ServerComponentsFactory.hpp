#pragma once

#include <Network/AbstractComponentsFactory.h>
#include "ServerPacketManager.h"

class CServerComponentsFactory : public Net::AbstractServerComponentsFactory
{
    public:
        std::shared_ptr<Net::CAbstractPacketManager> CreatePacketManager() override
        {
            return std::make_shared<CServerPacketManager>();
        }

        std::unique_ptr<Net::CAbstractPeerManager> CreatePeerManager() override
        {
            return std::make_unique<CPeerManager>();
        }
};
