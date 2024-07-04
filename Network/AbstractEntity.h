#pragma once

#include "PacketDefinition.h"
#include "Socket.h"

namespace Net
{
    class AbstractEntity
    {
        public:
            AbstractEntity() = default;
            virtual ~AbstractEntity() = 0;

            virtual bool Initialize(const char* c_szAddr, int port) = 0;
            virtual void Process() = 0;
            virtual bool Analyze(TPacketHeader header, std::shared_ptr<CSocket> socket) = 0;
    };

    inline AbstractEntity::~AbstractEntity() {}
}
