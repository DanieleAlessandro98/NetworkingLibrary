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
            virtual ~CAbstractPacketBaseHandler() = 0;

        public:
            virtual CSocket* GetSocket() = 0;
            virtual bool AnalyzePacket(TPacketHeader header) = 0;
    };

    class CAbstractPacketServerHandler
    {
        public:
            CAbstractPacketServerHandler() = default;
            virtual ~CAbstractPacketServerHandler() = 0;

        public:
            virtual bool Analyze(CAbstractPeer* peer, TPacketHeader header) = 0;
    };

    class CAbstractPacketClientHandler
    {
        public:
            CAbstractPacketClientHandler() = default;
            virtual ~CAbstractPacketClientHandler() = 0;

        public:
            virtual bool Analyze(TPacketHeader header) = 0;
    };

    inline CAbstractPacketBaseHandler::~CAbstractPacketBaseHandler() {}
    inline CAbstractPacketServerHandler::~CAbstractPacketServerHandler() {}
    inline CAbstractPacketClientHandler::~CAbstractPacketClientHandler() {}
}
