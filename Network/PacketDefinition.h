#pragma once

#include <cstdint>
#include <string>

namespace Net
{
    typedef uint8_t TPacketHeader;

    enum class PacketCGHeader : TPacketHeader
    {
        HEADER_CG_ACTION1 = 1,
        HEADER_CG_ACTION2 = 2,
    };

    enum class PacketGCHeader : TPacketHeader
    {
        HEADER_GC_SERVER_RESPONSE = 10,
    };

    struct TPacketCGAction1
    {
        PacketCGHeader header = PacketCGHeader::HEADER_CG_ACTION1;
        uint32_t numIntero;
    };

    struct TPacketCGAction2
    {
        PacketCGHeader header = PacketCGHeader::HEADER_CG_ACTION2;
        uint32_t numIntero;
    };

    struct TPacketGCServerResponse
    {
        PacketGCHeader header = PacketGCHeader::HEADER_GC_SERVER_RESPONSE;
        char response[50];
    };
}
