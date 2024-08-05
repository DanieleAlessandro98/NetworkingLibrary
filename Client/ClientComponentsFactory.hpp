#pragma once

#include <Network/AbstractComponentsFactory.h>
#include "ClientPacketManager.h"

class CClientComponentsFactory : public Net::AbstractClientComponentsFactory
{
    public:
        std::shared_ptr<Net::CAbstractPacketManager> CreatePacketManager() override
        {
            return std::make_shared<CClientPacketManager>();
        }
};
