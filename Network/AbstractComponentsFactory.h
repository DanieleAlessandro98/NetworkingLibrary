#pragma once

#include <memory>
#include "AbstractPacketManager.h"
#include "AbstractPeerManager.h"

namespace Net
{
    class CAbstractPacketManager;
    class CAbstractPeerManager;

    class AbstractClientComponentsFactory
    {
        public:
            virtual ~AbstractClientComponentsFactory() = default;

            virtual std::shared_ptr<CAbstractPacketManager> CreatePacketManager() = 0;
    };

    class AbstractServerComponentsFactory
    {
        public:
            virtual ~AbstractServerComponentsFactory() = default;

            virtual std::shared_ptr<CAbstractPacketManager> CreatePacketManager() = 0;
            virtual std::unique_ptr<CAbstractPeerManager> CreatePeerManager() = 0;
    };
}
