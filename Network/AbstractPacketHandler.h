#pragma once

#include "Socket.h"
#include "PacketDefinition.h"

namespace Net
{
    class CAbstractPeer;

    class CAbstractPacketBaseHandler
    {
        public:
            CAbstractPacketBaseHandler() = default;
            virtual ~CAbstractPacketBaseHandler() = default;

        public:
            virtual CSocket* GetSocket() = 0;
            virtual bool AnalyzePacket(TPacketHeader header) = 0;
    };

    class CAbstractPacketServerHandler
    {
        public:
            CAbstractPacketServerHandler() = default;
            virtual ~CAbstractPacketServerHandler() = default;

        public:
            virtual bool Analyze(CAbstractPeer* peer, TPacketHeader header) = 0;
    };

    class CAbstractPacketClientHandler
    {
        public:
            CAbstractPacketClientHandler() = default;
            virtual ~CAbstractPacketClientHandler() = default;

        public:
            virtual bool Analyze(TPacketHeader header) = 0;
    };
}
