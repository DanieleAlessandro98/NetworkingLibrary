#pragma once

#include <cstdint>
#include <string>

namespace Net
{
    typedef uint8_t TPacketHeader;

    enum class PacketHeader : TPacketHeader
    {
        HEADER_ACTION1,
        HEADER_ACTION2
    };

    struct TPacketAction1
    {
        PacketHeader header = PacketHeader::HEADER_ACTION1;
        uint32_t numIntero;
    };

    struct TPacketAction2
    {
        PacketHeader header = PacketHeader::HEADER_ACTION2;
        uint32_t numIntero;
    };
}
