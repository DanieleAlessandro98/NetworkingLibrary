#pragma once

#include "Socket.h"
#include "SocketWatcher.h"
#include "PacketDefinition.h"

namespace Net
{
    class SocketWatcher;

    class CAbstractPeer
    {
        public:
            CAbstractPeer() = default;
            virtual ~CAbstractPeer() = 0;

            virtual void SetPhase(int phase) = 0;
            virtual CSocket* GetSocket() = 0;
            virtual bool AnalyzePacket(TPacketHeader header) = 0;

            void AddSocketToWatcher(int fd);
            void RemoveSocketToWatcher(int fd);

        protected:
            std::shared_ptr<SocketWatcher>	m_serverWatcher;
    };

    inline CAbstractPeer::~CAbstractPeer() {}
}
