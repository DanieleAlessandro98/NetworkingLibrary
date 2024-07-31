#pragma once

#include "AbstractPeer.h"
#include "PacketDefinition.h"

namespace Net
{
    class CAbstractPacketServerHandler
    {
        public:
            CAbstractPacketServerHandler() = default;
            virtual ~CAbstractPacketServerHandler() = 0;

        public:
            virtual void Analyze(CAbstractPeer* peer, TPacketHeader header) = 0;
    };

    class CAbstractPacketClientHandler
    {
        public:
            CAbstractPacketClientHandler() = default;
            virtual ~CAbstractPacketClientHandler() = 0;

        public:
            virtual void Analyze(TPacketHeader header) = 0;
    };

    inline CAbstractPacketServerHandler::~CAbstractPacketServerHandler() {}
    inline CAbstractPacketClientHandler::~CAbstractPacketClientHandler() {}
}
