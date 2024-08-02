#pragma once

#include "Socket.h"
#include "SocketWatcher.h"
#include "PacketDefinition.h"
#include "AbstractPacketHandler.h"

namespace Net
{
    class SocketWatcher;
    class CAbstractPacketBaseHandler;

    class CAbstractPeer : public CAbstractPacketBaseHandler
    {
        public:
            CAbstractPeer() = default;
            virtual ~CAbstractPeer() = 0;

            virtual void SetPhase(int phase) = 0;

            void AddSocketToWatcher(int fd);
            void RemoveSocketToWatcher(int fd);

        protected:
            std::shared_ptr<SocketWatcher>	m_serverWatcher;
    };

    inline CAbstractPeer::~CAbstractPeer() {}
}
