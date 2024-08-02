#pragma once

#include <Network/AbstractComponentsFactory.h>
#include "ServerPacketManager.h"

class CServerComponentsFactory : public Net::AbstractComponentsFactory
{
    public:
        std::shared_ptr<Net::CAbstractPacketManager> CreatePacketManager() override
        {
            return std::make_shared<CServerPacketManager>();
        }
};
