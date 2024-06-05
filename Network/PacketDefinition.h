#pragma once

#include <cstdint>
#include <string>

namespace Net
{
    enum class PacketHeader : uint16_t
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
