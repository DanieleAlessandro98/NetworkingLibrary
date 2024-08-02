#pragma once

#include <memory>
#include "AbstractPacketManager.h"

namespace Net
{
    class CAbstractPacketManager;

    class AbstractComponentsFactory
    {
        public:
            virtual ~AbstractComponentsFactory() = default;

            virtual std::shared_ptr<CAbstractPacketManager> CreatePacketManager() = 0;
    };
}
